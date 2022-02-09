#include <avr/pgmspace.h>

extern "C" {
    #include <LUFA/Drivers/USB/USB.h>
    #include <LUFA/Platform/Platform.h>
}

// TYPEDEFS
typedef struct {
    USB_Descriptor_Configuration_Header_t Config;
    USB_Descriptor_Interface_t            HID_Interface;
    USB_HID_Descriptor_HID_t              HID_GenericHID;
    USB_Descriptor_Endpoint_t             HID_ReportINEndpoint;
} USB_Descriptor_Configuration_t;

enum StringDescriptors_t {
    STRING_ID_Language     = 0,
    STRING_ID_Manufacturer = 1,
    STRING_ID_Product      = 2,
};

// DESCRIPTORS
const USB_Descriptor_HIDReport_Datatype_t PROGMEM GenericReport[] =
{
	HID_DESCRIPTOR_VENDOR(0x00, 0x01, 0x02, 0x03, 214)
};

const USB_Descriptor_Device_t PROGMEM DeviceDescriptor = {
	.Header = {
        .Size = sizeof(USB_Descriptor_Device_t),
        .Type = DTYPE_Device
    },
	.USBSpecification = VERSION_BCD(1,1,0),
	.Class = USB_CSCP_NoDeviceClass,
	.SubClass = USB_CSCP_NoDeviceSubclass,
	.Protocol = USB_CSCP_NoDeviceProtocol,
	.Endpoint0Size = 8,
	.VendorID = 0x057E,
	.ProductID = 0x0337,
	.ReleaseNumber = VERSION_BCD(1,0,2),

	.ManufacturerStrIndex   = 1,
	.ProductStrIndex        = 2,
	.SerialNumStrIndex      = NO_DESCRIPTOR,

	.NumberOfConfigurations = 1
};

const USB_Descriptor_Configuration_t PROGMEM ConfigurationDescriptor = {
	.Config = {
        .Header = {
            .Size = sizeof(USB_Descriptor_Configuration_Header_t),
            .Type = DTYPE_Configuration
        },

        .TotalConfigurationSize = sizeof(USB_Descriptor_Configuration_t),
        .TotalInterfaces = 1,
        .ConfigurationNumber = 1,
        .ConfigurationStrIndex = NO_DESCRIPTOR,
        .ConfigAttributes = (USB_CONFIG_ATTR_RESERVED | USB_CONFIG_ATTR_SELFPOWERED),
        .MaxPowerConsumption = USB_CONFIG_POWER_MA(500)
    },

	.HID_Interface = {
        .Header = {
            .Size = sizeof(USB_Descriptor_Interface_t),
            .Type = DTYPE_Interface
        },
        .InterfaceNumber        = 0,
        .AlternateSetting       = 0x00,
        .TotalEndpoints         = 1,
        .Class                  = HID_CSCP_HIDClass,
        .SubClass               = HID_CSCP_NonBootSubclass,
        .Protocol               = HID_CSCP_NonBootProtocol,
        .InterfaceStrIndex      = NO_DESCRIPTOR
    },

	.HID_GenericHID = {
        .Header = {
            .Size = sizeof(USB_HID_Descriptor_HID_t),
            .Type = HID_DTYPE_HID
        },
        .HIDSpec                = VERSION_BCD(1,1,0),
        .CountryCode            = 0x00,
        .TotalReportDescriptors = 1,
        .HIDReportType          = HID_DTYPE_Report,
        .HIDReportLength        = 214
    },

	.HID_ReportINEndpoint = {
        .Header                 = {.Size = sizeof(USB_Descriptor_Endpoint_t), .Type = DTYPE_Endpoint},

        .EndpointAddress        = (ENDPOINT_DIR_IN | 1),
        .Attributes             = (EP_TYPE_INTERRUPT | ENDPOINT_ATTR_NO_SYNC | ENDPOINT_USAGE_DATA),
        .EndpointSize           = 8,
        .PollingIntervalMS      = 0x08
    },
};

const USB_Descriptor_String_t PROGMEM LanguageString = USB_STRING_DESCRIPTOR_ARRAY(LANGUAGE_ID_ENG);

const USB_Descriptor_String_t PROGMEM ManufacturerString = {
    .Header = {
        .Size = sizeof(USB_Descriptor_Header_t) + (sizeof(L"Nintendo") - 2),
        .Type = DTYPE_String
        },
    L"Nintendo"
};

const USB_Descriptor_String_t PROGMEM ProductString = {
    .Header = {
        .Size = sizeof(USB_Descriptor_Header_t) + (sizeof(L"WUP-028") - 2),
        .Type = DTYPE_String
        },
    L"WUP-028"
};