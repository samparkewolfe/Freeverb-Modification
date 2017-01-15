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

//This class is a combination of two comb filters to get a stereo effect
class stereoComb
:
public Component,
Slider::Listener
{
    
public:
    Slider tuningSlider;
    Label sliderLabel;
    //Left and right comb filters.
    comb combL, combR;
    //These are the buffers that the comb filters write too.
    std::vector<float> bufcombL, bufcombR;
    //The amount of stereo spread this will be variable unlike the "stereospread" variable which is constant and used to initialise bufcombR.
    int thisstereoSpread;
    
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
    void    setStereoSpread(const int& val);
    
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
    int thisstereoSpread;
    
    stereoAllpass();
    stereoAllpass(const stereoAllpass& copy);
    
    void sliderValueChanged(Slider* slider) override {};
    void resized() override;
    
    void	setbuffers(int size);
    void	mute();
    void	setfeedback(float val);
    float    processLeft(float val);
    float    processRight(float val);
    void    setStereoSpread(const int& val);
    
};


class revmodel
:
public Component,
public Slider::Listener,
Button::Listener
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
    void sliderValueChanged(Slider* slider) override
    {
        update();
    }
    void buttonClicked(Button* button) override
    {
        if(button == &originalParamsButton)
            setOriginalParameters();
    }
    
    void setOriginalParameters();
    
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
    
    static const int combtuning[8];
    static const int allpasstuning[4];
    
    int     numcombs    = 8;
    int     numallpasses  = 4;
    
    // Comb filters
    std::vector<stereoComb*> combs;
    // Allpass filters
    std::vector<stereoAllpass*> allpasses;
    
    GroupComponent group;
    Slider stereoSpreadSlider;
    Label stereoSpreadSliderLabel;
    TextButton originalParamsButton;
    
};

#endif//_revmodel_

//ends
