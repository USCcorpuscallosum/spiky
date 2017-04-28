#pragma once

#include <ofColor.h>

class ColorCycler {
public:
	enum Type { Constant, Loop, PingPong };

	ofFloatColor getColor() {
		if (mType != Constant && ofGetFrameNum() != mLastRecalculateFrame) {
			// Recalculate color
			float t = ofGetElapsedTimef() / mDuration;
			if (mType == Loop) {
				t = fmod(t, 1.0);
			} else if (mType == PingPong) {
				if (static_cast<int>(t) % 2 == 0) t = fmod(t, 1.0);
				else t = 1.0 - fmod(t, 1.0);
			}
			float hue = ofLerp(mStartHue, mEndHue, t);
			mColor = ofFloatColor::fromHsb(hue, mSaturation, mBrightness);
			mLastRecalculateFrame = ofGetFrameNum();
		}

		return mColor;
	}

	void setConstantColor(ofFloatColor color) {
		mType = Constant;
		mColor = color;
	}

	void setRainbow(float saturation, float brightness) {
		mType = Loop;
		mStartHue = 0.0;
		mEndHue = 1.0;
		mSaturation = saturation;
		mBrightness = brightness;
	}

	void setRange(float startHue, float endHue, float saturation, float brightness) {
		mType = PingPong;
		mStartHue = startHue;
		mEndHue = endHue;
		mSaturation = saturation;
		mBrightness = brightness;
	}

	void setDuration(float duration) { mDuration = duration; }

private:
	Type mType = Loop;
	ofColor mColor;

	float mStartHue = 0.0;
	float mEndHue = 1.0;
	float mSaturation = 1.0, mBrightness = 1.0;
	float mDuration = 3.0;

	uint64_t mLastRecalculateFrame = -1;

};
