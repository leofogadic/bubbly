#pragma once

#include <JuceHeader.h>
#include <iostream>
#include <vector>
#include "Bubble.h" 

#define N 50

using namespace juce;
using namespace std;

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class DistributionsComponent : public Component
{
public:
    //==============================================================================
    DistributionsComponent();
    ~DistributionsComponent() override;

    //==============================================================================
    void paint(juce::Graphics& g) override;
    void resized() override;
    Slider distributions[N];
    Label distributionLabels[N];


private:

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DistributionsComponent)
};
