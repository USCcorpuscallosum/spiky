#include "ofFmodSoundPlayerExtended.h"
#include "ofUtils.h"


static bool bFmodInitialized_ = false;
static unsigned int buffersize = 1024;

static FMOD_CHANNELGROUP * channelgroup;
static FMOD_SYSTEM       * sys;


void ofFmodExtendedSoundStopAll(){
	ofFmodSoundPlayerExtended::initializeFmod();
	FMOD_ChannelGroup_Stop(channelgroup);
}

void ofFmodExtendedSoundSetVolume(float vol){
	ofFmodSoundPlayerExtended::initializeFmod();
	FMOD_ChannelGroup_SetVolume(channelgroup, vol);
}

void ofFmodExtendedSoundUpdate(){
	if (bFmodInitialized_){
		FMOD_System_Update(sys);
	}
}

unsigned ofFmodExtendedGetBuffersize() {
	return buffersize;
}

void ofFmodExtendedSetBuffersize(unsigned int bs) {
	buffersize = bs;
}


FMOD_RESULT F_CALLBACK recordDSPReadCallback(FMOD_DSP_STATE *dsp_state, float *inbuffer, float *outbuffer, unsigned int length, int inchannels, int outchannels) {
	// Get the player object
	void *obj;
	FMOD_RESULT res = FMOD_DSP_GetUserData(dsp_state->instance, &obj);
	ofFmodSoundPlayerExtended* player = static_cast<ofFmodSoundPlayerExtended*>(obj);

	// Pass the buffer along
	player->receiveBuffer(inbuffer, length, inchannels);

	// Pass through
	memcpy(outbuffer, inbuffer, length * MIN(inchannels, outchannels) * sizeof(float));

	return FMOD_OK;
}


ofFmodSoundPlayerExtended::ofFmodSoundPlayerExtended(){
	bLoop 			= false;
	bLoadedOk 		= false;
	pan 			= 0.0; // range for oF is -1 to 1
	volume 			= 1.0f;
	internalFreq 	= 44100;
	speed 			= 1;
	bPaused 		= false;
	isStreaming		= false;
}

ofFmodSoundPlayerExtended::~ofFmodSoundPlayerExtended(){
	unload();
}



//---------------------------------------
// this should only be called once
void ofFmodSoundPlayerExtended::initializeFmod(){
	if(!bFmodInitialized_){
		
		FMOD_System_Create(&sys);
		
		// set buffersize, keep number of buffers
		unsigned int bsTmp;
		int nbTmp;
		FMOD_System_GetDSPBufferSize(sys, &bsTmp, &nbTmp);
		FMOD_System_SetDSPBufferSize(sys, buffersize, nbTmp);

		#ifdef TARGET_LINUX
			FMOD_System_SetOutput(sys,FMOD_OUTPUTTYPE_ALSA);
		#endif
		FMOD_System_Init(sys, 32, FMOD_INIT_NORMAL, nullptr);  //do we want just 32 channels?
		FMOD_System_GetMasterChannelGroup(sys, &channelgroup);
		bFmodInitialized_ = true;
	}
}




//---------------------------------------
void ofFmodSoundPlayerExtended::closeFmod(){
	if(bFmodInitialized_){
		FMOD_System_Close(sys);
		bFmodInitialized_ = false;
	}
}

//------------------------------------------------------------
bool ofFmodSoundPlayerExtended::load(string fileName, bool stream /*= false*/){

	fileName = ofToDataPath(fileName);

	// fmod uses IO posix internally, might have trouble
	// with unicode paths...
	// says this code:
	// http://66.102.9.104/search?q=cache:LM47mq8hytwJ:www.cleeker.com/doxygen/audioengine__fmod_8cpp-source.html+FSOUND_Sample_Load+cpp&hl=en&ct=clnk&cd=18&client=firefox-a
	// for now we use FMODs way, but we could switch if
	// there are problems:

	bMultiPlay = false;

	// [1] init fmod, if necessary

	initializeFmod();

	// [2] try to unload any previously loaded sounds
	// & prevent user-created memory leaks
	// if they call "loadSound" repeatedly, for example

	unload();

	// [3] load sound

	//choose if we want streaming
	int fmodFlags =  FMOD_SOFTWARE;
	if(stream)fmodFlags |= FMOD_CREATESTREAM;

	result = FMOD_System_CreateSound(sys, fileName.c_str(), fmodFlags, nullptr, &sound);

	if (result != FMOD_OK){
		bLoadedOk = false;
		ofLogError("ofFmodSoundPlayerExtended") << "load(): could not load \"" << fileName << "\"";
	} else {
		bLoadedOk = true;
		FMOD_Sound_GetLength(sound, &length, FMOD_TIMEUNIT_PCM);
		isStreaming = stream;
		isRecording = false;
	}

	return bLoadedOk;
}

bool ofFmodSoundPlayerExtended::record(int deviceId) {
	bMultiPlay = false;

	// [1] init fmod, if necessary
	initializeFmod();

	// [2] try to unload any previously loaded sounds
	// & prevent user-created memory leaks
	// if they call "loadSound" repeatedly, for example
	unload();

	int numDrivers = 0;
	result = FMOD_System_GetRecordNumDrivers(sys, &numDrivers);
	if (result != FMOD_OK) {
		ofLogError("ofFmodSoundPlayerExtended") << "record(): Could not get drivers";
	}
	ofLogNotice("ofFmodSoundPlayerExtended") << "record(): Found " << numDrivers << " drivers";

	if (numDrivers == 0) {
		ofLogError("ofFmodSoundPlayerExtended") << "record(): No recording devices found/plugged in!";
		bLoadedOk = false;
		return false;
	}

	// List drivers
	for (int i = 0; i < numDrivers; i++) {
		char name[64];
		result = FMOD_System_GetRecordDriverInfo(sys, i, name, sizeof(name), nullptr);
		if (result == FMOD_OK) {
			ofLogNotice("ofFmodSoundPlayerExtended") << "record(): " << i << ": " << name << (i == deviceId ? " *" : "");
		} else {
			ofLogWarning("ofFmodSoundPlayerExtended") << "record(): Could not get driver info for " << i;
		}
	}

	// Create user sound to record into, then start recording.
	int nativeRate = internalFreq;
	int nativeChannels = 2;
	FMOD_CREATESOUNDEXINFO exinfo = {0};
	exinfo.cbsize           = sizeof(FMOD_CREATESOUNDEXINFO);
	exinfo.numchannels      = nativeChannels;
	exinfo.format           = FMOD_SOUND_FORMAT_PCM16;
	exinfo.defaultfrequency = nativeRate;
	exinfo.length           = nativeRate * sizeof(short) * nativeChannels; // 1 second buffer, size here doesn't change latency

	result = FMOD_System_CreateSound(sys, 0, FMOD_HARDWARE | FMOD_LOOP_NORMAL | FMOD_OPENUSER, &exinfo, &sound);
	if (result != FMOD_OK) {
		ofLogError("ofFmodSoundPlayerExtended") << "record(): Could not create sound for device " << deviceId;
		bLoadedOk = false;
		return false;
	}

	result = FMOD_System_RecordStart(sys, deviceId, sound, true);
	if (result != FMOD_OK) {
		ofLogError("ofFmodSoundPlayerExtended") << "record(): Could not record device " << deviceId;
		bLoadedOk = false;
		return false;
	} else {
		// For some reason calling RecordStart again and sleeping for 60ms decreases the latency...
		result = FMOD_System_RecordStart(sys, deviceId, sound, true);
		usleep(1000*60);
		bLoadedOk = true;
		FMOD_Sound_GetLength(sound, &length, FMOD_TIMEUNIT_PCM);
		isStreaming = true;
		isRecording = true;
	}

	return bLoadedOk;
}

vector<string> ofFmodSoundPlayerExtended::getRecordingDeviceNames() {
	vector<string> names;

	int numDrivers = 0;
	FMOD_RESULT result = FMOD_System_GetRecordNumDrivers(sys, &numDrivers);

	for (int i = 0; i < numDrivers; i++) {
		char name[64];
		result = FMOD_System_GetRecordDriverInfo(sys, i, name, sizeof(name), nullptr);
		names.emplace_back(name);
	}

	return names;
}

//------------------------------------------------------------
void ofFmodSoundPlayerExtended::unload(){
	if (bLoadedOk){
		stop();						// try to stop the sound
		FMOD_Sound_Release(sound);
		bLoadedOk = false;
	}
}

//------------------------------------------------------------
bool ofFmodSoundPlayerExtended::isPlaying() const{

	if (!bLoadedOk) return false;

	int playing = 0;
	FMOD_Channel_IsPlaying(channel, &playing);
	return (playing != 0 ? true : false);
}

bool ofFmodSoundPlayerExtended::isPaused() const
{
	if (!bLoadedOk) return false;

	int paused = 0;
	FMOD_Channel_GetPaused(channel, &paused);
	return paused != 0;
}

//------------------------------------------------------------
float ofFmodSoundPlayerExtended::getSpeed() const{
	return speed;
}

//------------------------------------------------------------
float ofFmodSoundPlayerExtended::getPan() const{
	return pan;
}

//------------------------------------------------------------
float ofFmodSoundPlayerExtended::getVolume() const{
	return volume;
}

//------------------------------------------------------------
bool ofFmodSoundPlayerExtended::isLoaded() const{
	return bLoadedOk;
}

void ofFmodSoundPlayerExtended::receiveBuffer(float* buffer, unsigned int length, int channels)
{
	// Copy to the main buffer. Both are interleaved, so this is ok.
	soundBuffer.resize(length * channels);
	float* bufferData = soundBuffer.getBuffer().data();
	memcpy(bufferData, buffer, MIN(
		length * channels * sizeof(float),
		soundBuffer.size() * sizeof(float)
	));

	// Copy channel 0 to the mono buffer
	soundBufferMono.resize(length);
	bufferData = soundBufferMono.getBuffer().data();
	for (unsigned int i = 0; i < length; i++)
	{
		bufferData[i] = buffer[i * channels]; // skip other channels
	}
}

ofSoundBuffer& ofFmodSoundPlayerExtended::getCurrentSoundBuffer()
{
	return soundBuffer;
}

ofSoundBuffer& ofFmodSoundPlayerExtended::getCurrentSoundBufferMono()
{
	return soundBufferMono;
}

//------------------------------------------------------------
void ofFmodSoundPlayerExtended::setVolume(float vol){
	if (isPlaying()){
		FMOD_Channel_SetVolume(channel, vol);
	}
	volume = vol;
}

//------------------------------------------------------------
void ofFmodSoundPlayerExtended::setPosition(float pct){
	if (isPlaying()){
		int sampleToBeAt = (int)(length * pct);
		FMOD_Channel_SetPosition(channel, sampleToBeAt, FMOD_TIMEUNIT_PCM);
	}
}

void ofFmodSoundPlayerExtended::setPositionMS(int ms) {
	if (isPlaying()){
		FMOD_Channel_SetPosition(channel, ms, FMOD_TIMEUNIT_MS);
	}
}

//------------------------------------------------------------
float ofFmodSoundPlayerExtended::getPosition() const{
	if (isPlaying()){
		unsigned int sampleImAt;

		FMOD_Channel_GetPosition(channel, &sampleImAt, FMOD_TIMEUNIT_PCM);

		float pct = 0.0f;
		if (length > 0){
			pct = sampleImAt / (float)length;
		}
		return pct;
	} else {
		return 0;
	}
}

//------------------------------------------------------------
int ofFmodSoundPlayerExtended::getPositionMS() const{
	if (isPlaying()){
		unsigned int sampleImAt;

		FMOD_Channel_GetPosition(channel, &sampleImAt, FMOD_TIMEUNIT_MS);

		return sampleImAt;
	} else {
		return 0;
	}
}

//------------------------------------------------------------
void ofFmodSoundPlayerExtended::setPan(float p){
	pan = p;
	p = ofClamp(p, -1, 1);
	if (isPlaying()){
		FMOD_Channel_SetPan(channel,p);
	}
}


//------------------------------------------------------------
void ofFmodSoundPlayerExtended::setPaused(bool bP){
	if (isPlaying()){
		FMOD_Channel_SetPaused(channel,bP);
		bPaused = bP;
	}
}


//------------------------------------------------------------
void ofFmodSoundPlayerExtended::setSpeed(float spd){
	if (isPlaying()){
			FMOD_Channel_SetFrequency(channel, internalFreq * spd);
	}
	speed = spd;
}


//------------------------------------------------------------
void ofFmodSoundPlayerExtended::setLoop(bool bLp){
	if (isPlaying()){
		FMOD_Channel_SetMode(channel,  (bLp == true) ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);
	}
	bLoop = bLp;
}

// ----------------------------------------------------------------------------
void ofFmodSoundPlayerExtended::setMultiPlay(bool bMp){
	bMultiPlay = bMp;		// be careful with this...
}

// ----------------------------------------------------------------------------
void ofFmodSoundPlayerExtended::play(){

	// if it's a looping sound, we should try to kill it, no?
	// or else people will have orphan channels that are looping
	if (bLoop == true){
		FMOD_Channel_Stop(channel);
	}

	// if the sound is not set to multiplay, then stop the current,
	// before we start another
	if (!bMultiPlay){
		FMOD_Channel_Stop(channel);
	}

	FMOD_System_PlaySound(sys, FMOD_CHANNEL_FREE, sound, bPaused, &channel);

	FMOD_Channel_GetFrequency(channel, &internalFreq);
	FMOD_Channel_SetVolume(channel, volume);
	setPan(pan);
	FMOD_Channel_SetFrequency(channel, internalFreq * speed);
	FMOD_Channel_SetMode(channel, (bLoop || isRecording) ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);

	// Update sound buffers
	soundBuffer.setSampleRate(static_cast<unsigned int>(internalFreq * speed));
	soundBufferMono.setSampleRate(static_cast<unsigned int>(internalFreq * speed));

	// Create a recording DSP
	FMOD_DSP_DESCRIPTION dspDesc = { "Recorder" };
	dspDesc.version = 1;
	dspDesc.channels = 0;
	dspDesc.reset = nullptr;
	dspDesc.read = recordDSPReadCallback;
	dspDesc.numparameters = 0;
	dspDesc.userdata = this;
	result = FMOD_System_CreateDSP(sys, &dspDesc, &recordDSP);

	// Route the channel through the DSP
	FMOD_DSPCONNECTION *connection = nullptr;
	result = FMOD_Channel_AddDSP(channel, recordDSP, &connection);

	//fmod update() should be called every frame - according to the docs.
	//we have been using fmod without calling it at all which resulted in channels not being able
	//to be reused.  we should have some sort of global update function but putting it here
	//solves the channel bug
	FMOD_System_Update(sys);

}

// ----------------------------------------------------------------------------
void ofFmodSoundPlayerExtended::stop(){
	FMOD_Channel_Stop(channel);
}

