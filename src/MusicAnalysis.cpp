#include "MusicAnalysis.h"

MusicAnalysis::MusicAnalysis() {
	// Set up the frequency ranges for processing
	ranges.emplace(RANGE_BASS, Range(0.0, 200.0));
	ranges.emplace(RANGE_MID, Range(200.0, 600.0));
	ranges.emplace(RANGE_HIGHMID, Range(600.0, 6000.0));
	ranges.emplace(RANGE_HIGH, Range(6000.0, 22000.0));

	ofFmodExtendedSetBuffersize(BUFFER_SIZE);
	analyzer.setup(SAMPLE_RATE, BUFFER_SIZE, 1);
}

MusicAnalysis::~MusicAnalysis() {
	analyzer.exit();
}

void MusicAnalysis::setPlayer(ofFmodSoundPlayerExtended* player) {
	pause();
	currentPlayer = player;
	play();
}

void MusicAnalysis::play() {
	if (currentPlayer) {
		if (!currentPlayer->isPlaying()) currentPlayer->play();
		currentPlayer->setPaused(false);
	}
}

void MusicAnalysis::pause() {
	if (currentPlayer) {
		currentPlayer->setPaused(true);
	}
}

bool MusicAnalysis::isPaused() {
	return currentPlayer && currentPlayer->isPaused();
}

void MusicAnalysis::update() {
	if (!currentPlayer) return;

	// Get the current audio buffer
	ofSoundBuffer &buffer = currentPlayer->getCurrentSoundBufferMono();
	if (buffer.size() < BUFFER_SIZE) buffer.resize(BUFFER_SIZE); // make sure the buffer is not empty

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
	for (auto& pair : ranges) {
		getRangeVolume(pair.second, spectrum);
	}

	// Remap the spectrum to 0..1
	for (size_t i = 0; i < spectrum.size(); i++) {
		spectrum[i] = ofMap(spectrum[i], DB_MIN, DB_MAX, 0.0, 1.0, true); // map dBs to 0..1 and clamp
	}
}

void MusicAnalysis::getRangeVolume(Range &range, vector<float> &spectrum) {
	// Calculate the start and end bins in the spectrum
	int startIndex = static_cast<int>(range.minFreq / SAMPLE_RATE * spectrum.size());
	int endIndex = static_cast<int>(range.maxFreq / SAMPLE_RATE * spectrum.size());
	assert(startIndex >= 0 && startIndex < spectrum.size());
	assert(endIndex >= 0 && endIndex < spectrum.size());

	// If each bin has a max volume of 1.0, the max total volume is the number of bins
	range.maxVolume = endIndex - startIndex + 1.0;

	// Iterate through the spectrum range and add up the volumes
	range.volume = 0;
	for (int i = startIndex; i <= endIndex; i++) {
		range.volume += ofMap(spectrum[i], DB_MIN, DB_MAX, 0.0, 1.0, true); // map dBs to 0..1 and clamp
	}
}
