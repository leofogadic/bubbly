#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class Bubble: public juce::Timer
{
public:
    //==============================================================================
    Bubble(double r1, double lambdasRelative1, double lambdas1, double time1, double f01, double decay1, double sigma1, std::vector<double> ft1, std::vector<double> x1);
    Bubble();
    ~Bubble();
    void timerCallback();

    double a;
    double r;
    double lambdaRelative;
    double lambda;
    double time;
    double f0;
    double damping;
    double sigma;
    double D;
    std::vector<double> x;
    std::vector<double> ft;

    int counter = -1;
    int currentX[1000] = { 0 };
    int currentFt = 0;
    bool state[1000] = { false };

    int fs;

private:
    //==============================================================================
    // Your private member variables go here...



    //JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Bubble)
};
