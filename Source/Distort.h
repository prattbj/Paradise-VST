#pragma once
#define _USE_MATH_DEFINES
/*
	This namespace allows the processor to call these functions rather than
	having the math all done directly in the processor method.
*/
#include <cmath>
namespace distort
{
	//A gentle clipper
	float softClip(float input, float gain)
	{
		return (2.f / M_PI) * atan(input * gain);
	}

	//A not-so-gentle clipper
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

	//Distorts the audio using sinh inside of sin
	float sinHDistort(float input, float gain)
	{
		return sin(sinh(input * gain));
	}

	//Distorts the audio using 3 sin functions inside of another sin function
	float tripleSinDistort(float input, float gain)
	{
		return sin(sin(abs(input * input)) * sin(abs(input * gain)) * sin(abs(gain * gain)) * gain);
	}
	
	//Linear/sawtooth distortion
	float linearDistort(float input, float gain)
	{
		return (2 / M_PI) * asin(sin(input * gain));
	}
};

