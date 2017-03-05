#include "MusicAnalysis.h"
#include <iostream>

MusicAnalysis::MusicAnalysis()
{
	currentSong = NULL;
	sampleRate = 44100;
	isPlaying = false;

	// Get the sample rate for the player
	//shared_ptr<ofFmodSoundPlayer> fmodPlayer = std::static_pointer_cast<OF_SOUND_PLAYER_TYPE>(soundPlayer.getPlayer());
	//sampleRate = fmodPlayer->internalFreq;
}

//returns current volume
float MusicAnalysis::getVolume()
{
	return currentSong->getVolume();
}

//returns current pitch
float MusicAnalysis::getPitch()
{
	// TODO: implement
	return sinf(ofGetElapsedTimef());
}

//returns current beat
bool MusicAnalysis::getBeat()
{
	// TODO: implement
	return false;
}

//returns volume based on range of pitches given
float MusicAnalysis::getVolumeOfRange(float min, float max, float* outMaxVolume)
{
	float* spectrum = ofSoundGetSpectrum(SAMPLING_BANDS);

	// Get the start and end positions in the spectrum
	int startIndex = static_cast<int>(min / sampleRate * SAMPLING_BANDS);
	int endIndex = static_cast<int>(max / sampleRate * SAMPLING_BANDS);
	assert(startIndex < endIndex);
	assert(startIndex >= 0 && startIndex < SAMPLING_BANDS);
	assert(endIndex >= 0 && endIndex < SAMPLING_BANDS);

	if (outMaxVolume) {
		*outMaxVolume = endIndex - startIndex + 1.f;
	}

	// Iterate through the spectrum range and add up the volumes
	float volume = 0;
	for (int i = startIndex; i <= endIndex; i++)
	{
		volume += spectrum[i];
	}
	return volume;
}

void MusicAnalysis::loadSongs(vector<string> songs)
{
	for (int i = 0; i < songs.size(); i++)
	{
		ofSoundPlayer* tmp = new ofSoundPlayer();
		tmp->load(songs[i]);
		soundPlayer.push_back(tmp);
		numOfSongs++;
	}
}

void MusicAnalysis::changeSong(int index)
{
	if (soundPlayer.size() != 0 && soundPlayer.size() > index && index >= 0)
	{
		if (currentSong) currentSong->setPaused(true);
		currentSong = soundPlayer[index];
		currentSong->setPaused(false);
		currentSong->play();
	}
}

void MusicAnalysis::togglePlay()
{
	if (soundPlayer.size() != 0)
	{
		if (currentSong == NULL) currentSong = soundPlayer[0];
		
		isPlaying = !isPlaying;
		currentSong->setPaused(isPlaying);
		if (isPlaying) currentSong->play();
	}
}

