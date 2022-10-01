# Overview
This is a VST3 I created called Paradise. Based on the quality of the sound it outputs, it is anything but that.

I wanted to explore the JUCE framework and the math behind audio processing since I am a programmer but also enjoy making electronic music.
This was a somewhat successful endeavor since the result of it can actually do something. 

This program has 5 different types of distortion: soft clipping, hard clipping, linear, and two others I made up and thus don't have a name.
The functions I made to apply each of these are found in the Distort.h file.

[Software Demo Video](https://youtu.be/i4okrdUCJWg)

# Development Environment

C++, Visual Studio 2022, and [Projucer](https://juce.com/discover/projucer) were used to create this.

Projucer is an application that allows you to create many different types of projects using the JUCE framework 
to assist in creating audio applications, such as Virtual Studio Technology (VST). 

The CMath library was used for the audio processing, since trigonometric functions are needed to apply many
types of distortion.

# Useful Websites

* [JUCE documentation](https://docs.juce.com/master/index.html)
* [Stanford CCRMA](https://ccrma.stanford.edu/~jos/pasp/Virtual_Musical_Instruments.html) -Audio signal processing math

# Future Work

* Labels for each of the sliders need to be added
* Parallel processing rather than only sequential processing needs to be added
* Layout, color, etc... should be changed so that it looks nicer