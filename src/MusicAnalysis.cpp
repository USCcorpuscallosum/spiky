#include "MusicAnalysis.h"

MusicAnalysis::MusicAnalysis()
{
	currentSong = nullptr;

	// Set up the frequency ranges for processing
	ranges.emplace(RANGE_BASS, Range(0.0, 200.0));
	ranges.emplace(RANGE_MID, Range(200.0, 600.0));
	ranges.emplace(RANGE_HIGHMID, Range(600.0, 6000.0));
	ranges.emplace(RANGE_HIGH, Range(6000.0, 22000.0));

	sampleRate = 44100;
	bufferSize = 1024; // 512 bins
	smoothing = 0.4;

	ofFmodExtendedSetBuffersize(bufferSize);
	analyzer.setup(sampleRate, bufferSize, 1);
}

MusicAnalysis::~MusicAnalysis()
{
	for (size_t i = 0; i < soundPlayers.size(); i++)
	{
		delete soundPlayers[i];
	}

	analyzer.exit();
}

void MusicAnalysis::loadSongs(vector<string> songs)
{
	for (int i = 0; i < songs.size(); i++)
	{
		auto player = new ofFmodSoundPlayerExtended;
		player->load(songs[i]);
		soundPlayers.emplace_back(player);
	}
}

void MusicAnalysis::togglePlay()
{
	if (!soundPlayers.empty())
	{
		if (currentSong == nullptr)
		{
			currentSong = soundPlayers[0];
			currentSong->play();
		}
		else
		{
			currentSong->setPaused(!currentSong->isPaused());
		}
	}
}

void MusicAnalysis::changeSong(int index)
{
	if (soundPlayers.size() != 0 && index >= 0 && soundPlayers.size() > index)
	{
		if (currentSong) currentSong->setPaused(true);
		currentSong = soundPlayers[index];
		if (!currentSong->isPlaying()) currentSong->play();
		currentSong->setPaused(false);
	}
}

void MusicAnalysis::update()
{
	// Get the current audio buffer
	ofSoundBuffer &buffer = currentSong->getCurrentSoundBufferMono();
	if (buffer.size() < bufferSize) buffer.resize(bufferSize); // make sure the buffer is not empty

	// Run the actual analysis
	analyzer.analyze(buffer);

	// Get sound statistics
	// TODO: optimize by setting analyzer->setActive(0, ALG, false) for unused algorithms
	spectrum = analyzer.getValues(SPECTRUM, 0, smoothing);
	hpcp = analyzer.getValues(HPCP, 0, CLAMP(smoothing * 2.0, 0, 1)); // more smoothing, these jump a lot
	rms = analyzer.getValue(RMS, 0, smoothing);
	power = analyzer.getValue(POWER, 0, smoothing);
	pitchFreq = analyzer.getValue(PITCH_FREQ, 0, smoothing);
	hfc = analyzer.getValue(HFC, 0, smoothing);
	strongDecay = analyzer.getValue(STRONG_DECAY, 0, smoothing);
	strongDecayNorm = analyzer.getValue(STRONG_DECAY, 0, smoothing, true);
	onBeat = analyzer.getOnsetValue(0);

	// Update the Ranges using the frequency spectrum loudness
	for (auto& pair : ranges)
	{
		getRangeVolume(pair.second, spectrum);
	}

	// Remap the spectrum to 0..1
	for (size_t i = 0; i < spectrum.size(); i++)
	{
		spectrum[i] = ofMap(spectrum[i], DB_MIN, DB_MAX, 0.0, 1.0, true); // map dBs to 0..1 and clamp
	}
}

void MusicAnalysis::getRangeVolume(Range &range, vector<float> &spectrum)
{
	// Calculate the start and end bins in the spectrum
	int startIndex = static_cast<int>(range.minFreq / sampleRate * spectrum.size());
	int endIndex = static_cast<int>(range.maxFreq / sampleRate * spectrum.size());
	assert(startIndex >= 0 && startIndex < spectrum.size());
	assert(endIndex >= 0 && endIndex < spectrum.size());

	// If each bin has a max volume of 1.0, the max total volume is the number of bins
	range.maxVolume = endIndex - startIndex + 1.0;

	// Iterate through the spectrum range and add up the volumes
	range.volume = 0;
	for (int i = startIndex; i <= endIndex; i++)
	{
		range.volume += ofMap(spectrum[i], DB_MIN, DB_MAX, 0.0, 1.0, true); // map dBs to 0..1 and clamp
	}
}
