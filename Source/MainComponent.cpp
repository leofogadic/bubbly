#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize(580, 560);
    //setSize(1120, 600);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired(juce::RuntimePermissions::recordAudio)
        && !juce::RuntimePermissions::isGranted(juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request(juce::RuntimePermissions::recordAudio,
            [&](bool granted) { setAudioChannels(granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels(0, 2);
    }

    background = ImageFileFormat::loadFrom(BinaryData::bubbles_jpg, (size_t)BinaryData::bubbles_jpgSize);
    Image titleImage = ImageFileFormat::loadFrom(BinaryData::title_png, (size_t)BinaryData::title_pngSize);
    title.setImages(false, true, true, titleImage, 1.0f, {}, titleImage, 1.0f, {}, titleImage, 1.0f, {});

    addAndMakeVisible(alphaSlider);
    alphaSlider.setRange(0, 5.0, 0.1);
    alphaSlider.setValue(0.5);
    alphaSlider.addListener(this);

    addAndMakeVisible(alphaLabel);
    alphaLabel.setText("alpha", dontSendNotification);
    alphaLabel.attachToComponent(&alphaSlider, true);

    //=============================================

    addAndMakeVisible(betaSlider);
    betaSlider.setRange(0, 50, 0.1);
    betaSlider.setValue(0);
    betaSlider.addListener(this);

    addAndMakeVisible(betaLabel);
    betaLabel.setText("beta", dontSendNotification);
    betaLabel.attachToComponent(&betaSlider, true);

    //=============================================

    /*addAndMakeVisible(gammaSlider);
    gammaSlider.setRange(0, 5.0, 0.1);
    gammaSlider.setValue(5.0);
    gammaSlider.addListener(this);

    addAndMakeVisible(gammaLabel);
    gammaLabel.setText("gamma", dontSendNotification);
    gammaLabel.attachToComponent(&gammaSlider, true);*/

    //=============================================

    addAndMakeVisible(bubbleRateSlider);
    bubbleRateSlider.setRange(1, 1000, 1);
    bubbleRateSlider.setValue(10);
    bubbleRateSlider.addListener(this);

    addAndMakeVisible(bubbleRateLabel);
    bubbleRateLabel.setText("bbls/s", dontSendNotification);
    bubbleRateLabel.attachToComponent(&bubbleRateSlider, true);

    //=============================================

    addAndMakeVisible(rMinSlider);
    rMinSlider.setRange(0.2, 50.0, 0.2);
    rMinSlider.setValue(1.0);
    rMinSlider.addListener(this);

    addAndMakeVisible(rMinLabel);
    rMinLabel.setText("r-", dontSendNotification);
    rMinLabel.attachToComponent(&rMinSlider, true);

    //=============================================

    addAndMakeVisible(rMaxSlider);
    rMaxSlider.setRange(0.2, 50.0, 0.2);
    rMaxSlider.setValue(10.0);
    rMaxSlider.addListener(this);

    addAndMakeVisible(rMaxLabel);
    rMaxLabel.setText("r+", dontSendNotification);
    rMaxLabel.attachToComponent(&rMaxSlider, true);

    //=============================================

    addAndMakeVisible(xiSlider);
    xiSlider.setRange(0, 1, 0.01);
    xiSlider.setValue(0.1);
    xiSlider.addListener(this);

    addAndMakeVisible(xiLabel);
    xiLabel.setText("xi", dontSendNotification);
    xiLabel.attachToComponent(&xiSlider, true);

    //=============================================

    addAndMakeVisible(riseCutoffSlider);
    riseCutoffSlider.setRange(0, 1, 0.01);
    riseCutoffSlider.setValue(0.1);
    riseCutoffSlider.addListener(this);

    addAndMakeVisible(riseCutoffLabel);
    riseCutoffLabel.setText("rise", dontSendNotification);
    riseCutoffLabel.attachToComponent(&riseCutoffSlider, true);

    //=============================================

    addAndMakeVisible(distributionsComponent);
    distributionsComponent.setSize(700, 2500);

    viewport.setSize(400, 200);
    addAndMakeVisible(&viewport);
    viewport.setViewedComponent(&distributionsComponent);
    viewport.setScrollBarsShown(false, false, true, false);

    //=============================================

    rightArrowImage = ImageCache::getFromMemory(BinaryData::arrow_right_png, BinaryData::arrow_right_pngSize);
    leftArrowImage = ImageCache::getFromMemory(BinaryData::arrow_left_png, BinaryData::arrow_left_pngSize);

    showHideButton.setImages(false, true, true, rightArrowImage, 1.0f, {}, rightArrowImage, 1.0f, {}, rightArrowImage, 1.0f, {});

    //=============================================

    addAndMakeVisible(&title);

    //=============================================

    generateButton.onClick = [this] { generateButtonClicked(); };
    generateButton.setButtonText("Generate");
    generateButton.setColour(0, juce::Colour::fromFloatRGBA(0.0f, 0.0f, 0.0f, 0.5f));
    addAndMakeVisible(&generateButton);

    startButton.onClick = [this] { startButtonClicked(); };
    startButton.setButtonText("Start");
    startButton.setColour(1, juce::Colour::fromFloatRGBA(0.0f, 0.0f, 0.0f, 0.5f));
    addAndMakeVisible(&startButton);

    stopButton.onClick = [this] { stopButtonClicked(); };
    stopButton.setButtonText("Stop");
    stopButton.setColour(2, juce::Colour::fromFloatRGBA(0.0f, 0.0f, 0.0f, 0.5f));
    addAndMakeVisible(&stopButton);

    showHideButton.onClick = [this] { showHideButtonClicked(); };
    addAndMakeVisible(&showHideButton);

    generateBubbles();
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    fs = sampleRate;
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();


    float* outputL = bufferToFill.buffer->getWritePointer(0);
    float* outputR = bufferToFill.buffer->getWritePointer(1);


    for (int n = 0; n < bufferToFill.numSamples; n++)
    {
        float summedOutput = 0;
        if (playing)
        {
            for (int i = 0; i < N; i++)
            {
                for (int j = 0; j < bubbles[i].counter + 1; j++)
                {
                    if (bubbles[i].currentX[j] != fs && bubbles[i].state[j])
                    {
                        summedOutput += bubbles[i].x[bubbles[i].currentX[j]];
                        bubbles[i].currentX[j]++;

                        if (bubbles[i].currentX[j] == fs)
                        {
                            bubbles[i].counter--;
                            bubbles[i].state[j] = false;
                            bubbles[i].currentX[j] = 0;
                        }
                    }
                    else
                    {
                        bubbles[i].counter--;
                    }
                }
            }

            /*if (bubbleCreationRate > 100)
                summedOutput /= (float)bubbleCreationRate / 10.0f;*/

            summedOutput *= 0.5f;


            if (summedOutput > 1)
                summedOutput = 1;
            if (summedOutput < -1)
                summedOutput = -1;

        }

        outputL[n] = summedOutput;
        outputR[n] = outputL[n];
    }

}

void MainComponent::releaseResources()
{
}

//==============================================================================
void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    if (background.isValid())
        g.drawImageWithin(background, -50, -200, 1200, 800, RectanglePlacement::stretchToFit, false);

    g.setColour(juce::Colour::fromFloatRGBA(0.0f, 0.0f, 0.0f, 0.5f));
    g.fillRoundedRectangle(30, 80 + 40, 450, 410, 15);

    if (!hidden)
    {
        g.setColour(juce::Colour::fromFloatRGBA(0.0f, 0.0f, 0.0f, 0.5f));
        g.fillRoundedRectangle(550, 80 + 40, 430, 410, 15);
    }
}

void MainComponent::resized()
{
    int yDown = 40;

    alphaSlider.setBounds(100, 100 + yDown, 370, 20);
    betaSlider.setBounds(100, 150 + yDown, 370, 20);
    //gammaSlider.setBounds(100, 150 + yDown, 370, 20);
    bubbleRateSlider.setBounds(100, 200 + yDown, 370, 20);
    rMinSlider.setBounds(100, 250 + yDown, 370, 20);
    rMaxSlider.setBounds(100, 300 + yDown, 370, 20);
    xiSlider.setBounds(100, 350 + yDown, 370, 20);
    riseCutoffSlider.setBounds(100, 400 + yDown, 370, 20);

    title.setBounds(90, 15, 300, 100);

    viewport.setBounds(560, 80 + yDown, 430, 410);

    startButton.setBounds(90, 450 + yDown, 100, 30);
    stopButton.setBounds(210, 450 + yDown, 100, 30);
    generateButton.setBounds(330, 450 + yDown, 100, 30);
    showHideButton.setBounds(490, 305, 50, 50);

}

void MainComponent::generateBubbles()
{
    generating = true;
    playing = false;

    alpha = alphaSlider.getValue();
    gamma = gammaSlider.getValue();
    beta = betaSlider.getValue();
    bubbleCreationRate = bubbleRateSlider.getValue();
    rmin = rMinSlider.getValue() / 1000.0f;
    rmax = rMaxSlider.getValue() / 1000.0f;
    xi = xiSlider.getValue();
    riseCutoff = riseCutoffSlider.getValue();


    for (int i = 0; i < fs; i++)
        t.push_back(i / fs);


    double lambdaSum = 0;
    for (int i = 0; i < N; i++)
    {
        bubbles[i].stopTimer();
        bubbles[i].lambdaRelative = distributionsComponent.distributions[i].getValue();
        lambdaSum += bubbles[i].lambdaRelative;
    }

    double D = pow(((double)rand() / (RAND_MAX)), beta);

    for (int i = 0; i < N; i++)
    {
        std::uniform_int_distribution<> dis(0, 100);
        int rndnum = dis(generator);

        bubbles[i].D = pow((double)rndnum/100.0f, beta);

        double currentXi;
        if (bubbles[i].D > riseCutoff)
            currentXi = xi;
        else
            currentXi = 0;

        double exp = (double)(i + 1) / (double)N;
        bubbles[i].r = (rmin * pow((rmax / rmin), exp));
        bubbles[i].f0 = (3 / bubbles[i].r);
        bubbles[i].damping = (0.043 * bubbles[i].f0 + 0.0014 * pow(bubbles[i].f0, 3 / 2));
        bubbles[i].sigma = (currentXi * bubbles[i].damping);

        bubbles[i].ft.clear();
        for (int j = 0; j < fs; j++)
            bubbles[i].ft.push_back(bubbles[i].f0 * (1 + bubbles[i].sigma * t[j]));

        bubbles[i].lambda = (((double)bubbleCreationRate * bubbles[i].lambdaRelative) / lambdaSum);
        bubbles[i].time = (1 / bubbles[i].lambda) * 1000;


        bubbles[i].a = bubbles[i].D * pow(bubbles[i].r, alpha);

        bubbles[i].counter = 0;

        /*memset(bubbles[i].currentX, 0, sizeof(bubbles[i].currentX));
        memset(bubbles[i].state, false, sizeof(bubbles[i].state));*/
    }

    double amp = 1.0f / (5.0f * (double)bubbleCreationRate);

    for (int i = 0; i < N; i++)
    {
        bubbles[i].x.clear();
        for (int j = 0; j < fs; j++)
            bubbles[i].x.push_back(bubbles[i].a * sin(2 * juce::double_Pi * bubbles[i].ft[j] * t[j]) * exp(-bubbles[i].damping * t[j]));
    }

    generating = false;
}

void MainComponent::generateButtonClicked()
{
    generateBubbles();
}

void MainComponent::startButtonClicked()
{
    if (!generating)
    {
        playing = true;

        for (int i = 0; i < N; i++)
            bubbles[i].startTimer(bubbles[i].time);
    }
}

void MainComponent::stopButtonClicked()
{
    playing = false;

    for (int i = 0; i < N; i++)
        bubbles[i].stopTimer();
}

void MainComponent::showHideButtonClicked()
{

    if (hidden)
    {
        hidden = !hidden;
        repaint();
        setSize(1010, 560);
        showHideButton.setImages(false, true, true, leftArrowImage, 1.0f, {}, leftArrowImage, 1.0f, {}, leftArrowImage, 1.0f, {});
    }
    else
    {
        hidden = !hidden;
        repaint();
        setSize(580, 560);
        showHideButton.setImages(false, true, true, rightArrowImage, 1.0f, {}, rightArrowImage, 1.0f, {}, rightArrowImage, 1.0f, {});
    }

}

void MainComponent::sliderValueChanged(juce::Slider* slider)
{
}