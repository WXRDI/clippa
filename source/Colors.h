#pragma once
#include <juce_audio_processors/juce_audio_processors.h>

class CustomColors
{
public:

    // Gets you a color. Types:
    //      * bg
    //      * element1
    //      * control1
    // All types have soft clip and hard clip option
    juce::Colour Get(std::string type, bool isHard) {
            if (type == "bg" && isHard)
            {
                return bgColorHard;
            }
            else if (type == "bg" && !isHard)
            {
                return bgColorSoft;
            }

            if (type == "element1" && isHard)
            {
                return elementColorHard1;
            }
            else if (type == "element1" && !isHard)
            {
                return elementColorSoft1;
            }

            if (type == "control1" && isHard)
            {
                return controlColorHard1;
            }
            else if (type == "control1" && !isHard)
            {
                return controlColorSoft1;
            } else
            return ErrorColor;
    }

private:
    //soft mode
    juce::Colour bgColorSoft = juce::Colour (0xFFCAF0F8);
    juce::Colour elementColorSoft1 = juce::Colour (0xFF90E0EF);
    juce::Colour controlColorSoft1 = juce::Colour (0xFF48CAE4);

    //hard clip mode
    juce::Colour bgColorHard = juce::Colour (0xFFFFEAAE);
    juce::Colour elementColorHard1 = juce::Colour (0xFFFFC579);
    juce::Colour controlColorHard1 = juce::Colour (0xFFFF7D00);

    juce::Colour ErrorColor = juce::Colour (0xFFFFFFFF);
};