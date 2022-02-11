#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <string.h>

extern "C" {
    #include <LUFA/Drivers/USB/USB.h>
    #include <LUFA/Platform/Platform.h>
}

#include "wup/descriptors.h"
#include "wup/callbacks.h"

static uint8_t PrevHIDReportBuffer[214];

USB_ClassInfo_HID_Device_t Generic_HID_Interface = {
    .Config = {
        .InterfaceNumber = 0,
        .ReportINEndpoint = {
            .Address = (ENDPOINT_DIR_IN | 1), // From LUFA example. What is this and how should it be changed to point at my endpoint?
            .Size = 8,
            .Banks = 1,
        },
        .PrevReportINBuffer = PrevHIDReportBuffer,
        .PrevReportINBufferSize = sizeof(PrevHIDReportBuffer),
    },
};

int main(void)
{
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	clock_prescale_set(clock_div_1);
	USB_Init();
	GlobalInterruptEnable();

	for (;;)
	{
		HID_Device_USBTask(&Generic_HID_Interface);
		USB_USBTask();
	}
}

void EVENT_USB_Device_Connect(void)
{
}

void EVENT_USB_Device_Disconnect(void)
{
}

void EVENT_USB_Device_ConfigurationChanged(void)
{
	bool ConfigSuccess = true;

	ConfigSuccess &= HID_Device_ConfigureEndpoints(&Generic_HID_Interface);

	USB_Device_EnableSOFEvents();
}

void EVENT_USB_Device_ControlRequest(void)
{
	HID_Device_ProcessControlRequest(&Generic_HID_Interface);
}

void EVENT_USB_Device_StartOfFrame(void)
{
	HID_Device_MillisecondElapsed(&Generic_HID_Interface);
}