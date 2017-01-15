// Reverb model declaration
//
// Written by Jezar at Dreampoint, June 2000
// http://www.dreampoint.co.uk
// This code is public domain

#ifndef _revmodel_
#define _revmodel_

#include "comb.hpp"
#include "allpass.hpp"
#include "tuning.h"
#include "JuceHeader.h"
#include <array>


class stereoComb
:
public Component,
Slider::Listener
{
    
public:
    Slider tuningSlider;
    comb combL, combR;
    std::vector<float> bufcombL, bufcombR;
    
    stereoComb();
    stereoComb(const stereoComb& copy);
    
    void sliderValueChanged(Slider* slider) override {};
    void resized() override;
    
    void	setbuffers(int sizeL, int sizeR);
    void	mute();
    void	setdamp(float val);
    void	setfeedback(float val);
    float    processLeft(float val);
    float    processRight(float val);
    
};

class stereoAllpass
:
public Component,
Slider::Listener
{
    
public:
    Slider tuningSlider;
    allpass	allpassL, allpassR;
    std::vector<float> bufallpassL, bufallpassR;
    
    stereoAllpass();
    stereoAllpass(const stereoAllpass& copy);
    
    void sliderValueChanged(Slider* slider) override {};
    void resized() override;
    
    void	setbuffers(int sizeL, int sizeR);
    void	mute();
    void	setfeedback(float val);
    float    processLeft(float val);
    float    processRight(float val);
    
};





class revmodel
:
public Component
{
public:
					revmodel();
			void	mute();
			void	processmix(float *inputL, float *inputR, float *outputL, float *outputR, long numsamples, int skip);
			void	processreplace(float *inputL, float *inputR, float *outputL, float *outputR, long numsamples, int skip);
			void	setroomsize(float value);
			float	getroomsize();
			void	setdamp(float value);
			float	getdamp();
			void	setwet(float value);
			float	getwet();
			void	setdry(float value);
			float	getdry();
			void	setwidth(float value);
			float	getwidth();
			void	setmode(float value);
			float	getmode();
private:
			void	update();
private:
	float	gain;
	float	roomsize,roomsize1;
	float	damp,damp1;
	float	wet,wet1,wet2;
	float	dry;
	float	width;
	float	mode;
    int     index;
    
	// The following are all declared inline 
	// to remove the need for dynamic allocation
	// with its subsequent error-checking messiness

	// Allpass filters


    static const int combtuning[16];
    static const int allpasstuning[8];


//	// Buffers for the allpasses
//	float	bufallpassL1[allpasstuning[0]];
//	float	bufallpassR1[allpasstuning[1]];
//	float	bufallpassL2[allpasstuning[2]];
//	float	bufallpassR2[allpasstuning[3]];
//	float	bufallpassL3[allpasstuning[4]];
//	float	bufallpassR3[allpasstuning[5]];
//	float	bufallpassL4[allpasstuning[6]];
//	float	bufallpassR4[allpasstuning[7]];
    
    
    int     numcombs    = 8;
    int     numallpasses  = 4;
    
    // Comb filters
    std::vector<stereoComb*> combs;
    std::vector<stereoAllpass*> allpasses;
    
    
};

#endif//_revmodel_

//ends
