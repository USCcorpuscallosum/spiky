#include "MusicAnalysis.h"

MusicAnalysis::MusicAnalysis(string filename)
{
	samplingBands = 512;
	sound_player.load(filename);
	sound_player.play();
}

int getSamplingBands()
{
	return samplingBands;
}

//returns current volume
float MusicAnalysis::getVolume()
{
	return sound_player.getVolume();
}

//returns current pitch
float* MusicAnalysis::getPitch()
{
	return sound_player.ofSoundGetSpectrum();
}

//returns current beat
float MusicAnalysis::getBeat()
{
	return sound_player.getBeat();
}

//returns volume based on range of pitches given
float MusicAnalysis::getVolumeOfRange(float min, float max)
{
    
}

