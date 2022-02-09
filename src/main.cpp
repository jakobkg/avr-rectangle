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

/** Buffer to hold the previously generated HID report, for comparison purposes inside the HID class driver. */
static uint8_t PrevHIDReportBuffer[214];

/** LUFA HID Class driver interface configuration and state information. This structure is
 *  passed to all HID Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another.
 */
USB_ClassInfo_HID_Device_t Generic_HID_Interface = {
    .Config = {
        .InterfaceNumber = 0,
        .ReportINEndpoint = {
            .Address = (ENDPOINT_DIR_IN | 1),
            .Size = 8,
            .Banks = 1,
        },
        .PrevReportINBuffer = PrevHIDReportBuffer,
        .PrevReportINBufferSize = sizeof(PrevHIDReportBuffer),
    },
};

int main(void)
{
	/* Disable watchdog if enabled by bootloader/fuses */
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	/* Disable clock division */
	clock_prescale_set(clock_div_1);
	USB_Init();
	GlobalInterruptEnable();

	for (;;)
	{
		HID_Device_USBTask(&Generic_HID_Interface);
		USB_USBTask();
	}
}

/** Event handler for the library USB Connection event. */
void EVENT_USB_Device_Connect(void)
{
}

/** Event handler for the library USB Disconnection event. */
void EVENT_USB_Device_Disconnect(void)
{
}

/** Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged(void)
{
	bool ConfigSuccess = true;

	ConfigSuccess &= HID_Device_ConfigureEndpoints(&Generic_HID_Interface);

	USB_Device_EnableSOFEvents();
}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void)
{
	HID_Device_ProcessControlRequest(&Generic_HID_Interface);
}

/** Event handler for the USB device Start Of Frame event. */
void EVENT_USB_Device_StartOfFrame(void)
{
	HID_Device_MillisecondElapsed(&Generic_HID_Interface);
}