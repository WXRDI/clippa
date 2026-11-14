// VolumeKnob.h
#pragma once
#include <juce_audio_processors/juce_audio_processors.h>

class VolumeSlider : public juce::Component
{
public:
    VolumeSlider (juce::AudioProcessorValueTreeState& apvts)
        : attach (apvts, "volume", slider)
    {
        slider.setSliderStyle (juce::Slider::LinearBarVertical);
        slider.setTextBoxStyle (juce::Slider::NoTextBox, false, 90, 0);
        slider.setPopupDisplayEnabled (true, false, nullptr);

        // ставим ПОСЛЕ создания attachment, иначе он перезапишет
        slider.textFromValueFunction = [] (double v) {
            return juce::String ((int) v) + " Volume";
        };
        slider.valueFromTextFunction = [] (const juce::String& t) {
            return t.getDoubleValue();
        };

        // this function adds the slider to the editor
        addAndMakeVisible (&slider);

        // если нужна копия значения в переменной самого класса
        // slider.onValueChange = [this] { volume = (float) slider.getValue(); };
    }

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