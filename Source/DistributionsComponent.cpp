#include "DistributionsComponent.h"

//==============================================================================
DistributionsComponent::DistributionsComponent()
{
    //=============================================

    double initialValues[50] = {
        0.4300,
        0.4792,
        0.3937,
        0.2694,
        0.2176,
        0.1916,
        0.3057,
        0.2953,
        0.2745,
        0.3212,
        0.4300,
        0.5130,
        0.5544,
        0.5595,
        0.5130,
        0.3989,
        0.2538,
        0.2073,
        0.1968,
        0.1762,
        0.1864,
        0.2797,
        0.4300,
        0.5906,
        0.7149,
        0.7771,
        0.8030,
        0.8134,
        0.7512,
        0.6891,
        0.7461,
        0.8238,
        0.7305,
        0.6218,
        0.5285,
        0.3679,
        0.2849,
        0.1916,
        0.1606,
        0.1864,
        0.2124,
        0.2382,
        0.2953,
        0.2176,
        0.1606,
        0.1036,
        0.0776,
        0.0518,
        0.0418,
        0.0031
    };


    for (int i = 0; i < N; i++)
    {
        addAndMakeVisible(distributions[i]);
        distributions[i].setRange(0.0001, 1, 0.0001);
        distributions[i].setValue(initialValues[i]);


        addAndMakeVisible(distributionLabels[i]);
        distributionLabels[i].setText("r" + String(i + 1), dontSendNotification);
        distributionLabels[i].attachToComponent(&distributions[i], true);
    }

    //=============================================
}

DistributionsComponent::~DistributionsComponent()
{
}

//==============================================================================
void DistributionsComponent::paint(juce::Graphics& g)
{
}

void DistributionsComponent::resized()
{

    for (int i = 0; i < N; i++)
    {
        distributions[i].setBounds(40, 20 + (50 * i), 370, 20);
    }

}