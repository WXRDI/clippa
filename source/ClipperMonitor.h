#pragma once
#include <array>
#include <atomic>
#include <cmath>
#include <juce_gui_basics/juce_gui_basics.h>

class ClipperMonitor : public juce::Component, private juce::Timer
{
public:
    ClipperMonitor() { startTimerHz (60); }

    void setThreshold (float t) { threshold.store (juce::jlimit (0.01f, 4.0f, t)); }

    // audio from processBlock 
    void push (const float* in, const float* out, int numSamples) noexcept
    {
        for (int i = 0; i < numSamples; ++i)
        {
            accIn = juce::jmax (accIn, std::abs (in[i]));
            accOut = juce::jmax (accOut, std::abs (out[i]));

            if (++counter >= samplesPerColumn)
            {
                const int w = writePos.load (std::memory_order_relaxed);
                inBuf[(size_t) w] = accIn;
                outBuf[(size_t) w] = accOut;
                writePos.store ((w + 1) % columns, std::memory_order_release);
                accIn = accOut = 0.0f;
                counter = 0;
            }
        }
    }

    void paint (juce::Graphics& g) override
    {
        auto b = getLocalBounds().toFloat();
        g.setColour (juce::Colour (0xff15171c));
        g.fillRoundedRectangle (b, 6.0f);

        const float cy = b.getCentreY();
        const float half = b.getHeight() * 0.5f - 4.0f;
        const float colW = b.getWidth() / (float) columns;
        const float th = threshold.load();

        // mid line
        g.setColour (juce::Colours::white.withAlpha (0.08f));
        g.drawHorizontalLine ((int) cy, b.getX(), b.getRight());

        // 0 dBFS line
        const float zeroY = levelToPos (1.0f) * half;
        g.setColour (juce::Colours::white.withAlpha (0.12f));
        g.drawHorizontalLine ((int) (cy - zeroY), b.getX(), b.getRight());
        g.drawHorizontalLine ((int) (cy + zeroY), b.getX(), b.getRight());

        const int start = writePos.load (std::memory_order_acquire);

        for (int i = 0; i < columns; ++i)
        {
            const size_t idx = (size_t) ((start + i) % columns);
            const float x = b.getX() + (float) i * colW;
            const float pin = inBuf[idx];
            const float pout = outBuf[idx];

            // in
            g.setColour (juce::Colours::grey.withAlpha (0.45f));
            const float hin = levelToPos (pin) * half;
            g.fillRect (x, cy - hin, colW, hin * 2.0f);

            // out
            g.setColour (juce::Colour (0xff4fd1c5));
            const float hout = levelToPos (pout) * half;
            g.fillRect (x, cy - hout, colW, hout * 2.0f);

            // clip
            if (pin > th)
            {
                g.setColour (juce::Colours::red.withAlpha (0.8f));
                const float thY = levelToPos (th) * half;
                g.fillRect (x, cy - thY - 1.0f, colW, 2.0f);
                g.fillRect (x, cy + thY - 1.0f, colW, 2.0f);
            }
        }

        // clip lines
        g.setColour (juce::Colours::orange.withAlpha (0.8f));
        const float ty = levelToPos (th) * half;
        g.drawHorizontalLine ((int) (cy - ty), b.getX(), b.getRight());
        g.drawHorizontalLine ((int) (cy + ty), b.getX(), b.getRight());

        g.setColour (juce::Colours::white.withAlpha (0.15f));
        g.drawRoundedRectangle (b.reduced (0.5f), 6.0f, 1.0f);
    }

private:
    void timerCallback() override { repaint(); }

    static constexpr int columns = 512;

    static constexpr float minDb = -30.0f; // center of monitoring
    static constexpr float maxDb = 10.0f; // limit

    // linear scale
    static float levelToPos (float lin)
    {
        const float db = juce::Decibels::gainToDecibels (lin, minDb);
        return juce::jlimit (0.0f, 1.0f, juce::jmap (db, minDb, maxDb, 0.0f, 1.0f));
    }

    int samplesPerColumn = 256; // lower = faster

    std::array<float, (size_t) columns> inBuf {}, outBuf {};
    std::atomic<int> writePos { 0 };
    std::atomic<float> threshold { 1.0f };

    float accIn = 0.0f, accOut = 0.0f;
    int counter = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ClipperMonitor)
};