#pragma once
#include "Colors.h"
#include <juce_audio_processors/juce_audio_processors.h>

class OutGainKnob : public juce::Component
{
public:
    OutGainKnob(juce::AudioProcessorValueTreeState& apvts)
        : attach(apvts, "outGain", knob) 
    {
        knob.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
        knob.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
        knob.setPopupDisplayEnabled (true, false, nullptr);

        knob.textFromValueFunction = [] (double v) {
            return "+ " + juce::String ((float) v, 2) + " DB Out gain";
        };
        knob.valueFromTextFunction = [] (const juce::String& t) {
            return t.getDoubleValue();
        };

        addAndMakeVisible (knob);
    }
    ~OutGainKnob() {
        knob.setLookAndFeel (nullptr);
    }

    void resized() override { knob.setBounds (getLocalBounds()); }

    float getGain() const { return (float) knob.getValue(); }

private:
    juce::Slider knob;
    juce::AudioProcessorValueTreeState::SliderAttachment attach;
};