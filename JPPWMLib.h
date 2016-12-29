
#ifndef JPPWMLib_H
#define JPPWMLib_H

#include <Arduino.h>

enum Mode { COMMON_ANODE, COMMON_CATHODE };

class JPPWMLib
{
public:
        JPPWMLib(uint8_t bri, void (*pinset)(const int, const uint8_t), uint16_t PWMlevels, uint8_t _default_bri, uint8_t *_converttable);

        void loop(bool enable);

        void requestLastBri();
        void requestBri(uint8_t bri);

        void fadeToLastBri(uint16_t steps, unsigned long duration, unsigned long startdelay);
        void fadeToBri(uint8_t bri, uint16_t steps, unsigned long duration, unsigned long startdelay);

        uint8_t getBri(); //based on requesed, i.e. final one, not the current while fading
        uint8_t GetLastBri(); //returns bri to use for last bri command

        boolean getactivity();
        boolean getfading();

private:
        void setBri(uint8_t bri);
        void fadeTo(uint8_t bri, uint16_t steps, unsigned long duration, unsigned long startdelay);

        void (*PinWrite)(const int, const uint8_t);
        uint8_t levels;

        // pins for bri
        uint8_t bri_pin;

        // saves current state (bri)
        uint8_t curr_bri;

        // saves last non-zero state (bri)
        uint8_t last_bri;
        const uint8_t default_bri;

        uint32_t fadestarttime;
        uint32_t fadedelay;
        boolean fading;
        uint16_t fadesteps, currentfadestep, lastfadestep;
        uint8_t requested_bri;
        uint8_t start_bri;
        int32_t step_bri;

        uint32_t fadelastchecktime;

        uint8_t *converttable;
};
#endif
