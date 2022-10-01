#pragma once
#define _USE_MATH_DEFINES

#include <cmath>
namespace distort
{
	float softClip(float input, float gain)
	{
		return (2.f / M_PI) * atan(input * gain);
	}

	float hardClip(float input, float gain)
	{
		if (input >= 1.f / gain) {
			return 1.f;
		}
		else if (input <= 1.f / -gain) {
			return -1.f;
		}
		else {
			return input / (1.f / gain);
		}
	}

	float sinHDistort(float input, float gain)
	{
		return sin(sinh(input * gain));
	}

	float tripleSinDistort(float input, float gain)
	{
		return sin(sin(abs(input * input)) * sin(abs(input * gain)) * sin(abs(gain * gain)) * gain);
	}
	
	float linearDistort(float input, float gain)
	{
		return (2 / M_PI) * asin(sin(input * gain));
	}
};

