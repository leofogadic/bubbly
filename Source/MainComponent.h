#pragma once

#include <JuceHeader.h>
#include <iostream>
#include <vector>
#include <random>
#include "Bubble.h" 
#include "DistributionsComponent.h"

#define N 50

using namespace juce;
using namespace std;
//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent, public juce::Slider::Listener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

    void generateBubbles();
    void generateButtonClicked();
    void startButtonClicked();
    void stopButtonClicked();
    void showHideButtonClicked();

    void sliderValueChanged(juce::Slider* slider) override;

private:
    //==============================================================================
    // Your private member variables go here...

    Image background;
    ImageButton title;

    double fs;
    vector<double> t;

    double amplitude = 1;

    int bubbleCreationRate;

    double rmin;
    double rmax;
    double xi;
    double riseCutoff;
    double alpha;
    double beta;
    double gamma;
    double D;

    double level;

    Bubble bubbles[N];

    Slider alphaSlider;
    Slider gammaSlider;
    Slider betaSlider;
    Slider bubbleRateSlider;
    Slider rMinSlider;
    Slider rMaxSlider;
    Slider xiSlider;
    Slider riseCutoffSlider;

    Label alphaLabel;
    Label gammaLabel;
    Label betaLabel;
    Label bubbleRateLabel;
    Label rMinLabel;
    Label rMaxLabel;
    Label xiLabel;
    Label riseCutoffLabel;

    TextButton generateButton;
    TextButton startButton;
    TextButton stopButton;
    bool generating = false;
    bool playing = false;

    ImageButton showHideButton;
    bool hidden = true; 
    Image rightArrowImage;
    Image leftArrowImage;

    Viewport viewport;
    DistributionsComponent distributionsComponent;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};


static std::random_device rd;
static std::mt19937 generator(rd());