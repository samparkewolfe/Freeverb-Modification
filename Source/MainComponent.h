/*
  ==============================================================================

    MainComponent.h
    Created: 7 Jan 2017 10:03:53pm
    Author:  Samuel Thompson Parke-Wolfe

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "revmodel.hpp"


//==============================================================================
/*
 This component lives inside our window, and this is where you should put all
 your controls and content.
 */
class MainContentComponent   : public AudioAppComponent,
public Button::Listener,
public Slider::Listener

{
    revmodel model;
    
    ToggleButton modeToggle;    //Freeze Mode
    Slider roomSizeSlider,      //Room Size
    dampSlider,                 //Comb Dampening
    wetSlider,                  //Wet Mix
    drySlider,                  //Dry Mix
    widthSlider;                //Width
    
    Label roomSizeLabel,
    dampLabel,
    wetLabel,
    dryLabel,
    widthLabel;
    
    GroupComponent group;
    
public:
    
    MainContentComponent();
    ~MainContentComponent();
    
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked(Button* button) override;
    void sliderValueChanged(Slider* slider) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()     { return new MainContentComponent(); }


#endif  // MAINCOMPONENT_H_INCLUDED
