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
    Label sliderLabel;
    comb combL, combR;
    std::vector<float> bufcombL, bufcombR;
    
    stereoComb();
    stereoComb(const stereoComb& copy);
    
    void sliderValueChanged(Slider* slider) override {};
    void resized() override;
    
    void	setbuffers(int size);
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
    Label sliderLabel;
    allpass	allpassL, allpassR;
    std::vector<float> bufallpassL, bufallpassR;
    
    stereoAllpass();
    stereoAllpass(const stereoAllpass& copy);
    
    void sliderValueChanged(Slider* slider) override {};
    void resized() override;
    
    void	setbuffers(int size);
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
    
    void resized() override;
    
    
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

    static const int combtuning[8];
    static const int allpasstuning[4];
    
    int     numcombs    = 8;
    int     numallpasses  = 4;
    
    // Comb filters
    std::vector<stereoComb*> combs;
    // Allpass filters
    std::vector<stereoAllpass*> allpasses;
    
    
    GroupComponent group;
};

#endif//_revmodel_

//ends
