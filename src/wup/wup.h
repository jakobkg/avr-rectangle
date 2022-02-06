#ifndef PROTOCOL_USB_H
#define PROTOCOL_USB_H

#include <inttypes.h>

namespace Protocol {
    namespace USB {
        struct Config {
        uint16_t inEpMaxPacketSize;
        uint16_t inEpActualPacketSize;
        uint16_t outEpMaxPacketSize;
        uint8_t epOutId; // 1 or 2
        const char **descriptorStrings;
        uint16_t descriptorStringsLen;
        bool hid;
        uint16_t bcdHID;
        uint8_t* hidReportDescriptor;
        uint16_t hidReportDescriptorLen;
        bool useWinUSB;
        uint16_t VID;
        uint16_t PID;
        uint16_t bcdDevice;

        uint8_t* hidReportPtr;
        void (*reportActuationFunc)(void);
        };

        void start(Config);
    }
}

void setupUSB(void);

#endif