# Responsive Terrain

Built with [OpenFrameworks](http://openframeworks.cc/). Uses [ofxAudioAnalyzer](https://github.com/leozimmerman/ofxAudioAnalyzer) and [Essentia](http://essentia.upf.edu/) for realtime audio analysis.

## Setup
First, install [OpenFrameworks](http://openframeworks.cc/download/) and clone this repo inside your OF `apps/myApps` folder.

Then, [download ofxAudioAnalyzer](https://github.com/leozimmerman/ofxAudioAnalyzer/archive/master.zip). Extract it, rename the folder to "ofxAudioAnalyzer", and copy it into your OF `addons` folder.

On Windows, you don't have to install anything else! It should just build.

On macOS, you need to install Essentia first. Follow the "Install dependencies" section of [this guide](https://github.com/leozimmerman/ofxAudioAnalyzer/blob/8ab2d0686beb6cb798e2f391738a271f7caac79f/essentia_compilation.md#install-dependencies).

#### Regenerating Visual Studio Project
There are a couple changes that need to be made to the VS project after generating a new one using the projectGenerator:

1. Exclude all .cpp files from the `ofxAudioAnalyzer` addon.
2. Add the `ResponsiveTerrain.props` property sheet to all targets (see [Working with Project Properties](https://msdn.microsoft.com/en-us/library/669zx6zc.aspx#Build configurations)).
3. Add `ResponsiveTerrain.Debug.props` and `ResponsiveTerrain.Release.props` to the two Debug and two Release configurations respectively.
