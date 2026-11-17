// Now its a threshold
#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include "Colors.h"

class VolumeSlider : public juce::Component
{
public:
    VolumeSlider (juce::AudioProcessorValueTreeState& apvts)
        : attach (apvts, "threshold", slider)
    {
        CustomColors cc;
        slider.setColour (juce::Slider::trackColourId, cc.Get ("control1", true));
        slider.setColour (juce::Slider::backgroundColourId, cc.Get ("element1", true));

        slider.setSliderStyle (juce::Slider::LinearBarVertical);
        slider.setTextBoxStyle (juce::Slider::NoTextBox, false, 90, 0);
        slider.setPopupDisplayEnabled (true, false, nullptr);

        // ставим ПОСЛЕ создания attachment, иначе он перезапишет
        slider.textFromValueFunction = [] (double v) {
            return juce::String ((float) v, 2) + " DB Threshold";
        };
        slider.valueFromTextFunction = [] (const juce::String& t) {
            return t.getDoubleValue();
        };

        // this function adds the slider to the editor
        addAndMakeVisible (&slider);

        // если нужна копия значения в переменной самого класса
        // slider.onValueChange = [this] { volume = (float) slider.getValue(); };
    }

    ~VolumeSlider() override { slider.setLookAndFeel (nullptr); }

    float getVolume() const { return (float) slider.getValue(); }

    void resized() override
    {
        auto r = getLocalBounds();
        slider.setBounds (r);
    }

private:
    juce::Slider slider; // объявлен ДО attach, порядок важен
    juce::AudioProcessorValueTreeState::SliderAttachment attach;
};