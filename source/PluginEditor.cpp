#include "PluginEditor.h"

PluginEditor::PluginEditor (PluginProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p), volumeSlider(p.apvts)
{
    juce::ignoreUnused (processorRef);

    //addAndMakeVisible (inspectButton);

    // this chunk of code instantiates and opens the melatonin inspector
    inspectButton.onClick = [&] {
        if (!inspector)
        {
            inspector = std::make_unique<melatonin::Inspector> (*this);
            inspector->onClose = [this]() { inspector.reset(); };
        }

        inspector->setVisible (true);
    };

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    addAndMakeVisible (volumeSlider);
}

PluginEditor::~PluginEditor()
{
}



void PluginEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::white);

    //auto area = getLocalBounds();
    g.setColour (juce::Colours::black);
    g.setFont (16.0f);
            // Advanced way to make text
            // auto helloWorld = juce::String ("Hello from ") + PRODUCT_NAME_WITHOUT_VERSION + " v" VERSION + " running in " + CMAKE_BUILD_TYPE;
            // g.drawText (helloWorld, area.removeFromTop (150), juce::Justification::centred, false);
}

void PluginEditor::resized()
{
    // layout the positions of your child components here
    auto area = getLocalBounds();
    area.removeFromBottom(50);
    inspectButton.setBounds (getLocalBounds().withSizeKeepingCentre(100, 50));
    volumeSlider.setBounds (40, 30, 20, getHeight() - 60);
}
