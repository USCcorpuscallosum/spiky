#include "MusicAnalysis.h"

MusicAnalysis::MusicAnalysis(string filename)
{
	soundPlayer.load(filename);
	soundPlayer.play();

	// Get the sample rate for the player
	//shared_ptr<ofFmodSoundPlayer> fmodPlayer = std::static_pointer_cast<OF_SOUND_PLAYER_TYPE>(soundPlayer.getPlayer());
	//sampleRate = fmodPlayer->internalFreq;
	sampleRate = 44100;
}

//returns current volume
float MusicAnalysis::getVolume()
{
	return soundPlayer.getVolume();
}

//returns current pitch
float MusicAnalysis::getPitch()
{
	// TODO: implement
	return 0;
}

//returns current beat
bool MusicAnalysis::getBeat()
{
	// TODO: implement
	return false;
}

//returns volume based on range of pitches given
float MusicAnalysis::getVolumeOfRange(float min, float max)
{
	float* spectrum = ofSoundGetSpectrum(SAMPLING_BANDS);

	// Get the start and end positions in the spectrum
	int startIndex = static_cast<int>(min / sampleRate * SAMPLING_BANDS);
	int endIndex = static_cast<int>(max / sampleRate * SAMPLING_BANDS);
	assert(startIndex < endIndex);
	assert(startIndex >= 0 && startIndex < SAMPLING_BANDS);
	assert(endIndex >= 0 && endIndex < SAMPLING_BANDS);

	// Iterate through the spectrum range and add up the volumes
	float volume = 0;
	for (int i = startIndex; i <= endIndex; i++)
	{
		volume += spectrum[i];
	}
	return volume;
}
