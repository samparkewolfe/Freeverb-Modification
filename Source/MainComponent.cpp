/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

MainContentComponent::MainContentComponent()
{
    setSize (400, 400);
    
    // specify the number of input and output channels that we want to open
    setAudioChannels (2, 2);
    
    
    addAndMakeVisible(&model);
    
    addAndMakeVisible(&group);
    group.setText("Reverb Front End");
    group.setColour(GroupComponent::outlineColourId, Colours::grey);
    group.toFront(false);
    
    addAndMakeVisible(&modeToggle);
    modeToggle.addListener(this);
    modeToggle.setButtonText("Freeze");
    
    addAndMakeVisible(&roomSizeSlider);
    roomSizeSlider.addListener(this);
    roomSizeSlider.setSliderStyle(Slider::LinearBarVertical);
    roomSizeSlider.setRange(0.0, 1.0);
    roomSizeSlider.setValue(0.5);
    
    addAndMakeVisible(&dampSlider);
    dampSlider.addListener(this);
    dampSlider.setSliderStyle(Slider::LinearBarVertical);
    dampSlider.setRange(0.0, 1.0);
    dampSlider.setValue(0.5);
    
    addAndMakeVisible(&wetSlider);
    wetSlider.addListener(this);
    wetSlider.setSliderStyle(Slider::LinearBarVertical);
    wetSlider.setRange(0.0, 1.0);
    wetSlider.setValue(0.5);
    
    addAndMakeVisible(&drySlider);
    drySlider.addListener(this);
    drySlider.setSliderStyle(Slider::LinearBarVertical);
    drySlider.setRange(0.0, 1.0);
    drySlider.setValue(0.5);
    
    addAndMakeVisible(&widthSlider);
    widthSlider.addListener(this);
    widthSlider.setSliderStyle(Slider::LinearBarVertical);
    widthSlider.setRange(0.0, 1.0);
    widthSlider.setValue(0.5);
    
    addAndMakeVisible(&roomSizeLabel);
    roomSizeLabel.setText("Room Size", dontSendNotification);
    roomSizeLabel.attachToComponent (&roomSizeSlider, false);
    
    addAndMakeVisible(&dampLabel);
    dampLabel.setText("Damp", dontSendNotification);
    dampLabel.attachToComponent (&dampSlider, false);

    
    addAndMakeVisible(&wetLabel);
    wetLabel.setText("Wet", dontSendNotification);
    wetLabel.attachToComponent (&wetSlider, false);
    
    addAndMakeVisible(&dryLabel);
    dryLabel.setText("Dry", dontSendNotification);
    dryLabel.attachToComponent (&drySlider, false);

    addAndMakeVisible(&widthLabel);
    widthLabel.setText("Width", dontSendNotification);
    widthLabel.attachToComponent (&widthSlider, false);

    
}


MainContentComponent::~MainContentComponent()
{
    shutdownAudio();
}

void MainContentComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{

}

void MainContentComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    float** inputs = bufferToFill.buffer->getArrayOfWritePointers();
    float** outputs = bufferToFill.buffer->getArrayOfWritePointers();
    
    model.processreplace(inputs[0],inputs[1],outputs[0],outputs[1],bufferToFill.numSamples,1);
    
}

void MainContentComponent::releaseResources()
{

}

void MainContentComponent::paint (Graphics& g)
{
}

void MainContentComponent::resized()
{
    
    Rectangle<int> areaModel(getLocalBounds());
    areaModel.removeFromBottom(getHeight()/2);
    areaModel.reduce(10, 10);
    model.setBounds(areaModel);
    
    
    Rectangle<int> areaSliders(getLocalBounds());
    areaSliders.removeFromTop(getHeight()/2);
    
    areaSliders.reduce(10, 10);
    group.setBounds(areaSliders);
    areaSliders.reduce(10, 10);
    areaSliders.removeFromTop(30);
    
    int sliderGap = 10;
    int sliderWidth = areaSliders.getWidth()/6 - 10;
    modeToggle.setBounds(areaSliders.removeFromLeft(sliderWidth));
    areaSliders.removeFromLeft(sliderGap);
    roomSizeSlider.setBounds(areaSliders.removeFromLeft(sliderWidth));
    areaSliders.removeFromLeft(sliderGap);
    dampSlider.setBounds(areaSliders.removeFromLeft(sliderWidth));
    areaSliders.removeFromLeft(sliderGap);
    wetSlider.setBounds(areaSliders.removeFromLeft(sliderWidth));
    areaSliders.removeFromLeft(sliderGap);
    drySlider.setBounds(areaSliders.removeFromLeft(sliderWidth));
    areaSliders.removeFromLeft(sliderGap);
    widthSlider.setBounds(areaSliders.removeFromLeft(sliderWidth));
    
}

void MainContentComponent::buttonClicked(juce::Button *button)
{
    if(button == &modeToggle)   model.setmode(button->getToggleState());
}

void MainContentComponent::sliderValueChanged(juce::Slider *slider)
{
    if(slider == & roomSizeSlider)  model.setroomsize(slider->getValue());
    if(slider == & dampSlider)      model.setdamp(slider->getValue());
    if(slider == & wetSlider)       model.setwet(slider->getValue());
    if(slider == & drySlider)       model.setdry(slider->getValue());
    if(slider == & widthSlider)     model.setwidth(slider->getValue());
}
