# Spiky

A spiky and smooth and colorful music visualizer. Visualizes audio in real time from files or input devices.
[@usccorpuscallosum](https://github.com/USCcorpuscallosum) Spring 2017.

Built with [OpenFrameworks](http://openframeworks.cc/). Uses [ofxAudioAnalyzer](https://github.com/leozimmerman/ofxAudioAnalyzer) and [Essentia](http://essentia.upf.edu/) for audio analysis.

![Screenshot](images/screenshot1.png)

![Screenshot](images/screenshot3.png)

![Screenshot](images/screenshot-lsd1.png)

## Controls

- `l` = toggle LSD mode
- `space` = play/pause
- `1-9` = play song by number
- `0` = play from audio input

## Setup
First, install [OpenFrameworks](http://openframeworks.cc/download/) and clone this repo inside your OF `apps/myApps` folder.

Then, [download ofxAudioAnalyzer](https://github.com/leozimmerman/ofxAudioAnalyzer/archive/master.zip). Extract it, rename the folder to "ofxAudioAnalyzer", and copy it into your OF `addons` folder.

#### Regenerating Visual Studio Project

If you need to regenerate the VS project with the projectGenerator, there are a couple changes that need to be made afterwards:

1. Exclude all .cpp files from the `ofxAudioAnalyzer` addon.
2. Add the `ResponsiveTerrain.props` property sheet to all targets (see [Working with Project Properties](https://msdn.microsoft.com/en-us/library/669zx6zc.aspx#Build configurations)).
3. Add `ResponsiveTerrain.Debug.props` and `ResponsiveTerrain.Release.props` to the two Debug and two Release configurations respectively.
