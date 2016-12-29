#include "JPPWMLib.h"

JPPWMLib::JPPWMLib(uint8_t bri, void (*pinset)(const int, const uint8_t), uint16_t PWMlevels, uint8_t _default_bri, uint8_t *_converttable) :

        default_bri(_default_bri)

{
        bri_pin = bri;
        fading = false;
        PinWrite = pinset;
        levels = PWMlevels;
        requested_bri = 0;
        curr_bri = 0;
        converttable = _converttable;
        last_bri = (uint8_t)((uint16_t)default_bri * levels / 100);
}

uint8_t JPPWMLib::GetLastBri()
{
        return (uint8_t)((uint16_t)last_bri * 100 / levels);
}

// Set LED-color to custom color instantely
void JPPWMLib::setBri(uint8_t bri)
{
        // set color of LED
        PinWrite(bri_pin, bri);

        // save state
        curr_bri = bri;
}

// Fade to custom color in specific time in specific steps
void JPPWMLib::fadeTo(uint8_t bri, uint16_t steps, unsigned long duration, unsigned long startdelay)
{
        if (bri != requested_bri)
        {
                requested_bri = bri;
                if (converttable[bri] != curr_bri)
                {
                        fadesteps = steps;

                        start_bri = curr_bri;

                        // calculate the width of each step
                        step_bri = ((int32_t)(converttable[bri] - curr_bri) << 8) / steps;

                        lastfadestep = 0;
                        fading = true;
                        fadedelay = duration / steps;
                        fadestarttime = millis() + startdelay;
                        fadelastchecktime = millis();
                }
                else
                        fading = false;
        }
}

void JPPWMLib::requestBri(uint8_t bri)
{
        if (!bri && requested_bri)
        {
                last_bri = requested_bri;
        }
        fadeTo((uint8_t)((uint16_t)bri * levels / 100), 1, 1, 0);
}

void JPPWMLib::requestLastBri()
{
        if (!getactivity())
                fadeTo(last_bri, 1, 1, 0);
}

// Fade to custom color in specific time in specific steps
void JPPWMLib::fadeToBri(uint8_t bri, uint16_t steps, unsigned long duration, unsigned long startdelay)
{
        if (!bri && requested_bri)
        {
                last_bri = requested_bri;
        }
        fadeTo((uint8_t)((uint16_t)bri * levels / 100), steps, duration, startdelay);
}

void JPPWMLib::fadeToLastBri(uint16_t steps, unsigned long duration, unsigned long startdelay)
{
        if (!getactivity())
                fadeTo(last_bri, steps, duration, startdelay);
}

uint8_t JPPWMLib::getBri()
{
        return (uint8_t)((uint16_t)requested_bri * 100 / levels);
}

boolean JPPWMLib::getactivity()
{
        return (requested_bri || fading);
}

boolean JPPWMLib::getfading()
{
        return fading;
}
void JPPWMLib::loop(bool enable)
{
        if (fading)
        {
                if (enable)
                {
                        fadelastchecktime = millis();
                        if ((signed long)(millis() - fadestarttime) > 0)
                        {
                                currentfadestep = ((unsigned long)(millis() - fadestarttime)) / fadedelay;
                                if (currentfadestep >= fadesteps)
                                {
                                        fading = false;
                                        setBri(converttable[requested_bri]);
                                }
                                else if (currentfadestep != lastfadestep)
                                {
                                        //set color of current step
                                        setBri(((((int32_t)(start_bri)) << 8) + (currentfadestep * step_bri)) >> 8);
                                        lastfadestep = currentfadestep;
                                }
                        }
                }
                else
                {
                        fadestarttime += (unsigned long)(millis() - fadelastchecktime);
                        fadelastchecktime = millis();
                }
        }
}
