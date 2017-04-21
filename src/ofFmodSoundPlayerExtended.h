#pragma once

#include "ofConstants.h"
#include "ofBaseSoundPlayer.h"
#include <ofSoundBuffer.h>


extern "C" {
#include "fmod.h"
#include "fmod_errors.h"
}


void ofFmodExtendedSoundStopAll();
void ofFmodExtendedSoundSetVolume(float vol);
void ofFmodExtendedSoundUpdate(); // calls FMOD update.
unsigned int ofFmodExtendedGetBuffersize();
void ofFmodExtendedSetBuffersize(unsigned int bs);


// Based on ofFmodSoundPlayer. Adds the ability to get the current buffer.
class ofFmodSoundPlayerExtended : public ofBaseSoundPlayer {
public:
	ofFmodSoundPlayerExtended();
	virtual ~ofFmodSoundPlayerExtended();

	bool load(string fileName, bool stream = false);
	bool record(int deviceId);
	void unload();
	void play();
	void stop();

	void setVolume(float vol);
	void setPan(float vol);
	void setSpeed(float spd);
	void setPaused(bool bP);
	void setLoop(bool bLp);
	void setMultiPlay(bool bMp);
	void setPosition(float pct); // 0 = start, 1 = end;
	void setPositionMS(int ms);

	float getPosition() const;
	int getPositionMS() const;
	bool isPlaying() const;
	bool isPaused() const;
	float getSpeed() const;
	float getPan() const;
	float getVolume() const;
	bool isLoaded() const;

	ofSoundBuffer& getCurrentSoundBuffer();
	ofSoundBuffer& getCurrentSoundBufferMono();
	void receiveBuffer(float* buffer, unsigned int length, int channels);

	static void initializeFmod();
	static void closeFmod();
	static vector<string> getRecordingDeviceNames();

private:
	bool isStreaming;
	bool isRecording;
	bool bMultiPlay;
	bool bLoop;
	bool bLoadedOk;
	bool bPaused;
	float pan; // -1 to 1
	float volume; // 0 - 1
	float internalFreq; // 44100 ?
	float speed; // -n to n, 1 = normal, -1 backwards
	unsigned int length; // in samples;

	FMOD_RESULT result;
	FMOD_CHANNEL * channel;
	FMOD_SOUND * sound;
	FMOD_DSP * recordDSP;

	ofSoundBuffer soundBuffer;
	ofSoundBuffer soundBufferMono;

};
