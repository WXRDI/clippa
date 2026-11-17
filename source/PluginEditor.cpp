#include "PluginEditor.h"
#include "Colors.h"

PluginEditor::PluginEditor (PluginProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p), volumeSlider (p.apvts), outGainKnob(p.apvts)
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
    setSize (500, 300);
    addAndMakeVisible (volumeSlider);

    addAndMakeVisible (processorRef.monitor);
    processorRef.monitor.setBounds (70, 40, 400, 150);
    addAndMakeVisible (outGainKnob);
    outGainKnob.setBounds (70, processorRef.monitor.getBottom() + 10, 100, 100);
}

PluginEditor::~PluginEditor()
{
    removeChildComponent (&processorRef.monitor);
    removeChildComponent (&outGainKnob);
}

void PluginEditor::paint (juce::Graphics& g)
{
    CustomColors cc = CustomColors();
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (cc.Get ("bg", true));

    g.setColour (cc.Get ("element1", true));
    g.fillRect (0, 0, 500, 30);

    auto area = getLocalBounds();
    auto text = area.removeFromTop (30);

    g.setColour (juce::Colours::white);

    g.setFont (20.0f);
    g.setFont (juce::FontOptions (16.0f).withStyle ("Bold"));
    auto clippaText = juce::String ("Clippa");
    g.drawText (clippaText, 10, 5, 100, 20, juce::Justification::left, false);


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
    volumeSlider.setBounds (30, 40, 20, getHeight() - 60);
}
