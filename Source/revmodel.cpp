// Reverb model implementation
//
// Written by Jezar at Dreampoint, June 2000
// http://www.dreampoint.co.uk
// This code is public domain

#include "revmodel.hpp"



// These values assume 44.1KHz sample rate
// they will probably be OK for 48KHz sample rate
// but would need scaling for 96KHz (or other) sample rates.
// The values were obtained by listening tests.

int const revmodel::combtuning[] =
{
    1116,
    1188,
    1277,
    1356,
    1422,
    1491,
    1557,
    1617,
};

int const revmodel::allpasstuning[] =
{
    556,
    441,
    341,
    225,
};


//Double
stereoComb::stereoComb()
{
    addAndMakeVisible(&tuningSlider);
    tuningSlider.addListener(this);
    tuningSlider.setSliderStyle(Slider::LinearBarVertical);
    tuningSlider.setRange(0.0, 1.0);
    tuningSlider.setValue(0.5);
    
    addAndMakeVisible(&sliderLabel);
    sliderLabel.setText("Combfilter", dontSendNotification);
    sliderLabel.attachToComponent(&tuningSlider, false);
}

stereoComb::stereoComb(const stereoComb& copy)
{
    combL = copy.combL;
    combR = copy.combR;
    bufcombL = copy.bufcombL;
    bufcombR = copy.bufcombR;
}

void stereoComb::setbuffers(int size)
{
    for(int i = 0; i<size; i++)
    {
        bufcombL.push_back(0.0f);
    }
    
    for(int i = 0; i<size+stereospread; i++)
    {
        bufcombR.push_back(0.0f);
    }
    
    combL.setbuffer(bufcombL.data(),size);
    combR.setbuffer(bufcombR.data(),size+stereospread);
}

void stereoComb::mute()
{
    combL.mute();
    combR.mute();
}

void stereoComb::setdamp(float val)
{
    combL.setdamp(val);
    combR.setdamp(val);
}

void stereoComb::setfeedback(float val)
{
    combL.setfeedback(val);
    combR.setfeedback(val);
}

float stereoComb::processLeft(float val)
{
    return combL.process(val);
}

float stereoComb::processRight(float val)
{
    return combR.process(val);
}

void stereoComb::resized()
{
    Rectangle<int> area(getLocalBounds());
    area.removeFromTop(20);
    area.reduce(10, 10);
    tuningSlider.setBounds(area);
}





stereoAllpass::stereoAllpass()
{
    addAndMakeVisible(&tuningSlider);
    tuningSlider.addListener(this);
    tuningSlider.setSliderStyle(Slider::LinearBarVertical);
    tuningSlider.setRange(0.0, 1.0);
    tuningSlider.setValue(0.5);
    
    addAndMakeVisible(&sliderLabel);
    sliderLabel.setText("Allpass Filter", dontSendNotification);
    sliderLabel.attachToComponent(&tuningSlider, false);
}

stereoAllpass::stereoAllpass(const stereoAllpass& copy)
{
    allpassL = copy.allpassL;
    allpassR = copy.allpassR;
    bufallpassL = copy.bufallpassL;
    bufallpassR = copy.bufallpassR;
}

void stereoAllpass::setbuffers(int size)
{
    for(int i = 0; i<size; i++)
    {
        bufallpassL.push_back(0.0f);
    }
    
    for(int i = 0; i<size+stereospread; i++)
    {
        bufallpassR.push_back(0.0f);
    }
    
    allpassL.setbuffer(bufallpassL.data(),size);
    allpassR.setbuffer(bufallpassR.data(),size+stereospread);
}

void stereoAllpass::mute()
{
    allpassL.mute();
    allpassR.mute();
}

void stereoAllpass::setfeedback(float val)
{
    allpassL.setfeedback(val);
    allpassR.setfeedback(val);
}

float stereoAllpass::processLeft(float val)
{
    return allpassL.process(val);
}

float stereoAllpass::processRight(float val)
{
    return allpassR.process(val);
}

void stereoAllpass::resized()
{
    Rectangle<int> area(getLocalBounds());
    area.removeFromTop(20);
    area.reduce(10, 10);
    tuningSlider.setBounds(area);
}




revmodel::revmodel()
{
    
    addAndMakeVisible(&group);
    group.setText("Reverb Back End");
    group.setColour(GroupComponent::outlineColourId, Colours::grey);
    group.toFront(false);
    
    // Tie the components to their buffers
    for(int i = 0; i<numcombs; i++)
    {
        //stereoComb c;
        combs.push_back(new stereoComb());
        combs[i]->setbuffers(combtuning[i]);
        addAndMakeVisible(combs[i]);
    }
    
    for(int i = 0; i<numallpasses; i++)
    {
        //stereoAllpass a;
        allpasses.push_back(new stereoAllpass());
        allpasses[i]->setbuffers(allpasstuning[i]);
        allpasses[i]->setfeedback(0.5f);
        addAndMakeVisible(allpasses[i]);
    }
    
	setwet(initialwet);
	setroomsize(initialroom);
	setdry(initialdry);
	setdamp(initialdamp);
	setwidth(initialwidth);
	setmode(initialmode);

	// Buffer will be full of rubbish - so we MUST mute them
	mute();
}

void revmodel::mute()
{
	if (getmode() >= freezemode)
		return;

	for (int i=0;i<numcombs;i++)
	{
		combs[i]->mute();
	}
	for (int i=0;i<numallpasses;i++)
	{
		allpasses[i]->mute();
	}
}

void revmodel::processreplace(float *inputL, float *inputR, float *outputL, float *outputR, long numsamples, int skip)
{
	float outL,outR,input;

	while(numsamples-- > 0)
	{
		outL = outR = 0;
		input = (*inputL + *inputR) * gain;

		// Accumulate comb filters in parallel
		for(int i=0; i<numcombs; i++)
		{
            outL += combs[i]->processLeft(input);
            outR += combs[i]->processRight(input);
		}

		// Feed through allpasses in series
		for(int i=0; i<numallpasses; i++)
		{
			outL = allpasses[i]->processLeft(outL);
			outR = allpasses[i]->processRight(outR);
		}

		// Calculate output REPLACING anything already there
		*outputL = outL*wet1 + outR*wet2 + *inputL*dry;
		*outputR = outR*wet1 + outL*wet2 + *inputR*dry;

		// Increment sample pointers, allowing for interleave (if any)
		inputL += skip;
		inputR += skip;
		outputL += skip;
		outputR += skip;
        index++;
	}
}

void revmodel::update()
{
// Recalculate internal values after parameter change

	int i;

	wet1 = wet*(width/2 + 0.5f);
	wet2 = wet*((1-width)/2);

	if (mode >= freezemode)
	{
		roomsize1 = 1;
		damp1 = 0;
		gain = muted;
	}
	else
	{
		roomsize1 = roomsize;
		damp1 = damp;
		gain = fixedgain;
	}

	for(i=0; i<numcombs; i++)
	{
		combs[i]->setfeedback(roomsize1);
	}

	for(i=0; i<numcombs; i++)
	{
		combs[i]->setdamp(damp1);
	}
}

// The following get/set functions are not inlined, because
// speed is never an issue when calling them, and also
// because as you develop the reverb model, you may
// wish to take dynamic action when they are called.

void revmodel::setroomsize(float value)
{
	roomsize = (value*scaleroom) + offsetroom;
	update();
}

float revmodel::getroomsize()
{
	return (roomsize-offsetroom)/scaleroom;
}

void revmodel::setdamp(float value)
{
	damp = value*scaledamp;
	update();
}

float revmodel::getdamp()
{
	return damp/scaledamp;
}

void revmodel::setwet(float value)
{
	wet = value*scalewet;
	update();
}

float revmodel::getwet()
{
	return wet/scalewet;
}

void revmodel::setdry(float value)
{
	dry = value*scaledry;
}

float revmodel::getdry()
{
	return dry/scaledry;
}

void revmodel::setwidth(float value)
{
	width = value;
	update();
}

float revmodel::getwidth()
{
	return width;
}

void revmodel::setmode(float value)
{
	mode = value;
	update();
}

float revmodel::getmode()
{
	if (mode >= freezemode)
		return 1;
	else
		return 0;
}

void revmodel::resized()
{
    //add or minus combs
    //Combs
    //add or minus all passes
    //Allpasses
    //reset button
    //Stereo spread
    
    Rectangle<int> area(getLocalBounds());
    group.setBounds(area);
    area.reduce(10, 10);
    
    
    Rectangle<int> areaCombs(area);
    areaCombs.removeFromBottom(area.getHeight()/2);
    int sliderWidth = (areaCombs.getWidth()/numcombs) ;
    for(int i = 0; i<numcombs; i++)
    {
        combs[i]->setBounds(areaCombs.removeFromLeft(sliderWidth));
    }
    
    
    Rectangle<int> areaAllpass(area);
    areaAllpass.removeFromTop(area.getHeight()/2);
    sliderWidth = (areaAllpass.getWidth()/numallpasses);
    for(int i = 0; i<numallpasses; i++)
    {
        allpasses[i]->setBounds(areaAllpass.removeFromLeft(sliderWidth));
    }
}

//ends
