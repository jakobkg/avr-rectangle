#include "wup.h"

#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/io.h>
#include <avr/power.h>
#include <LUFA/Drivers/USB/USB.h>

struct device_config {
    const USB_Descriptor_Device_t *device_descriptor;
    const USB_Descriptor_Interface_t *interface_descriptor;
    const USB_Descriptor_Configuration_Header_t *config_descriptor;
    const USB_HID_StdDescriptor_HID_t *hid_descriptor;
};

const USB_Descriptor_Device_t device_descriptor = {
    .Header = {
        .Size = sizeof(USB_Descriptor_Device_t),
        .Type = DTYPE_Device
    },
    .USBSpecification = VERSION_BCD(2,0,0),
    .Class = 0,
    .SubClass = 0,
    .Protocol = 0,
    .Endpoint0Size = 64,
    .VendorID = 0x057E,
    .ProductID = 0x0337,
    .ReleaseNumber = 0x102,
    .ManufacturerStrIndex = 1,
    .ProductStrIndex = 2,
    .SerialNumStrIndex = 3,
    .NumberOfConfigurations = 1
};

const USB_Descriptor_Interface_t interface_descriptor = {
    .Header = {
        .Size = sizeof(USB_Descriptor_Interface_t),
        .Type = DTYPE_Interface
    },
    .InterfaceNumber = 0,
    .AlternateSetting = 0,
    .TotalEndpoints = 2,
    .Class = 3,
    .SubClass = 0,
    .Protocol = 0,
    .InterfaceStrIndex = 0
};

const USB_Descriptor_Configuration_Header_t config_descriptor = {
    .Header = {
        .Size = sizeof(USB_Descriptor_Configuration_Header_t),
        .Type = DTYPE_Configuration
    },
    .TotalConfigurationSize = sizeof(USB_Descriptor_Configuration_Header_t)
                            + sizeof(USB_Descriptor_Interface_t)
                            + sizeof(USB_HID_StdDescriptor_HID_t)
                            + sizeof(USB_Descriptor_Endpoint_t)
                            + sizeof(USB_Descriptor_Endpoint_t),
    .TotalInterfaces = 1,
    .ConfigurationNumber = 1,
    .ConfigurationStrIndex = 0,
    .ConfigAttributes = 0xE0,
    .MaxPowerConsumption = 0xFA
};

const USB_HID_StdDescriptor_HID_t hid_descriptor = {
    .bLength = 9,
    .bDescriptorType = 33,
    .bcdHID = VERSION_BCD(1,0,0), //TODO find right value
    .bCountryCode = 0,
    .bNumDescriptors = 1,
    .bDescriptorType2 = 34,
    .wDescriptorLength = 214
};

const USB_Descriptor_Endpoint_t endpoint_in_descriptor = {
    .Header = {
        .Size = sizeof(USB_Descriptor_Endpoint_t),
        .Type = DTYPE_Endpoint
    },
    .EndpointAddress = 129,
    .Attributes = 0, // TODO FIND VALUE
    .EndpointSize = 37,
    .PollingIntervalMS = 8
};

USB_Descriptor_Endpoint_t endpoint_out_descriptor = {
    .Header = {
        .Size = sizeof(USB_Descriptor_Endpoint_t),
        .Type = DTYPE_Endpoint
    },
    .EndpointAddress = 2,
    .Attributes = 0, // TODO FIND VALUE
    .EndpointSize = 5,
    .PollingIntervalMS = 8
};

const USB_Descriptor_String_t PROGMEM LangString = USB_STRING_DESCRIPTOR_ARRAY(LANGUAGE_ID_ENG);

const USB_Descriptor_String_t PROGMEM ManuString = {
    {
        .Size = sizeof(USB_Descriptor_Header_t) + (sizeof(L"Nintendo") - 2),
        .Type = DTYPE_String
    },
    L"Nintendo"
};

const USB_Descriptor_String_t PROGMEM ProductString = {
    {
        .Size = sizeof(USB_Descriptor_Header_t) + (sizeof(L"WUP-028") - 2),
        .Type = DTYPE_String
    },
    L"WUP-028"
};

void setupUSB(void) {
    MCUSR = MCUSR & ~(1 << WDRF);
    wdt_disable();
    clock_prescale_set(clock_div_1);

    USB_Init();
}