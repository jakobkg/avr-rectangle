#include <inttypes.h>

struct __attribute__((packed)) controllerReport_t {
    uint8_t controllerType;
    uint8_t A:1; uint8_t B:1; uint8_t X:1; uint8_t Y:1;
    uint8_t Left:1; uint8_t Right:1; uint8_t Down:1; uint8_t Up:1;
    uint8_t Start:1; uint8_t Z:1; uint8_t R:1; uint8_t L:1;
    uint8_t _padding:4;
    uint8_t stickX; uint8_t stickY;
    uint8_t cX; uint8_t cY;
    uint8_t analogL; uint8_t analogR;
};

const controllerReport_t PROGMEM defaultNoControllerReport = {
    .controllerType = 0x00, // 0x00 = disconnected, 0x10 = connected
    .A = 0, .B = 0, .X = 0, .Y = 0,
    .Left = 0, .Right = 0, .Down = 0, .Up = 0,
    .Start = 0, .Z = 0, .R = 0, .L = 0,
    ._padding = 0,
    .stickX = 0, .stickY = 0,
    .cX = 0, .cY = 0,
    .analogL = 0, .analogR = 0
};

const controllerReport_t PROGMEM defaultControllerReport = {
    .controllerType = 0x10, // 0x00 = disconnected, 0x10 = connected
    .A = 0, .B = 0, .X = 0, .Y = 0,
    .Left = 0, .Right = 0, .Down = 0, .Up = 0,
    .Start = 0, .Z = 0, .R = 0, .L = 0,
    ._padding = 0,
    .stickX = 128, .stickY = 128,
    .cX = 128, .cY = 128,
    .analogL = 0, .analogR = 0
};

struct adapterReport_t {
    uint8_t thirtythree;
    controllerReport_t controllerReports[4];
};

uint16_t reportSize = sizeof(adapterReport_t);

adapterReport_t defaultReport = {
    .thirtythree = 33,
    .controllerReports = {
        defaultControllerReport,
        defaultNoControllerReport,
        defaultNoControllerReport,
        defaultNoControllerReport
    }
};