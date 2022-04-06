#include "Bubble.h"

//==============================================================================
Bubble::Bubble(double r1, double lambdasRelative1, double lambdas1, double time1, double f01, double decay1, double sigma1, std::vector<double> ft1, std::vector<double> x1)
{
    r = r1;
    lambdaRelative = lambdasRelative1;
    lambda = lambdas1;
    time = time1;
    f0 = f01;
    damping = decay1;
    sigma = sigma1;

    ft = ft1;
    x = x1; 
}

Bubble::Bubble()
{
}

Bubble::~Bubble()
{
    stopTimer();
}

void Bubble::timerCallback()
{
    counter++;

    state[counter] = true;
    currentX[counter] = 0;
}
