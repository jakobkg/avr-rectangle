#include "wup.h"

#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/io.h>
#include <avr/power.h>

extern "C" {
    #include <LUFA/Drivers/USB/USB.h>
}


typedef struct {
	USB_Descriptor_Configuration_Header_t Config;
	// Generic HID Interface
	USB_Descriptor_Interface_t            HID_Interface;
	USB_HID_Descriptor_HID_t              HID_VendorHID;
	USB_Descriptor_Endpoint_t             HID_ReportINEndpoint;
} USB_Descriptor_Configuration_t;

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

const USB_HID_Descriptor_HID_t hid_descriptor = {
    .Header {
        .Size = sizeof(USB_HID_Descriptor_HID_t),
        .Type = HID_DTYPE_HID
    },
    .HIDSpec = VERSION_BCD(1,1,0),
    .CountryCode = 0,
    .TotalReportDescriptors = 1,
    .HIDReportType = 34,
    .HIDReportLength = 214
};

const USB_Descriptor_Endpoint_t endpoint_in_descriptor = {
    .Header = {
        .Size = sizeof(USB_Descriptor_Endpoint_t),
        .Type = DTYPE_Endpoint
    },
    .EndpointAddress = 129,
    .Attributes = (EP_TYPE_INTERRUPT | ENDPOINT_ATTR_NO_SYNC | ENDPOINT_USAGE_DATA),
    .EndpointSize = 37,
    .PollingIntervalMS = 8
};

const USB_Descriptor_Endpoint_t endpoint_out_descriptor = {
    .Header = {
        .Size = sizeof(USB_Descriptor_Endpoint_t),
        .Type = DTYPE_Endpoint
    },
    .EndpointAddress = 2,
    .Attributes = (EP_TYPE_INTERRUPT | ENDPOINT_ATTR_NO_SYNC | ENDPOINT_USAGE_DATA),
    .EndpointSize = 5,
    .PollingIntervalMS = 8
};

const USB_Descriptor_Configuration_t device_config {
    .Config = config_descriptor,
    .HID_Interface = interface_descriptor,
    .HID_VendorHID = hid_descriptor,
    .HID_ReportINEndpoint = endpoint_in_descriptor,
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

extern "C" {
    uint16_t CALLBACK_USB_GetDescriptor(const uint16_t wValue,
                                        const uint16_t wIndex,
                                        const void** const DescriptorAddress) {
        const uint8_t  DescriptorType   = (wValue >> 8);
        const uint8_t  DescriptorNumber = (wValue & 0xFF);

        const void* Address = NULL;
        uint16_t    Size    = 0;

        switch (DescriptorType) {
            case DTYPE_Device:
                Address = &device_descriptor;
                Size    = sizeof(USB_Descriptor_Device_t);
                break;
            case DTYPE_Configuration:
                Address = &device_config;
                Size    = sizeof(USB_Descriptor_Configuration_t);
                break;
            case DTYPE_String:
                switch (DescriptorNumber) {
                    case 0x00:
                        Address = &LangString;
                        Size    = pgm_read_byte(&LangString.Header.Size);
                        break;
                    case 0x01:
                        Address = &ManuString;
                        Size    = pgm_read_byte(&ManuString.Header.Size);
                        break;
                    case 0x02:
                        Address = &ProductString;
                        Size    = pgm_read_byte(&ProductString.Header.Size);
                        break;
                }

                break;
        }

        *DescriptorAddress = Address;
        return Size;
    }
}