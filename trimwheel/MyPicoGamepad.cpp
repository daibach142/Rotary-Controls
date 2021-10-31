/*
 * Copyright (c) 2018-2019, Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
 /*
  * Modified by diabach142 2021 to produce a no-axis 2 button joystick.
  * This to simplify code for use as a trimwheel via a Rotary Encoder
  */
  

#include "stdint.h"
#include "PluggableUSBHID.h"
#include "MyPicoGamepad.h"
#include "usb_phy_api.h"

#define REPORT_ID_KEYBOARD 1
#define REPORT_ID_VOLUME 3

using namespace arduino;


PicoGamepad::PicoGamepad(bool connect, uint16_t vendor_id, uint16_t product_id, uint16_t product_release) : USBHID(get_usb_phy(), 0, 0, vendor_id, product_id, product_release)
{
    //_lock_status = 0;
}

PicoGamepad::PicoGamepad(USBPhy *phy, uint16_t vendor_id, uint16_t product_id, uint16_t product_release) : USBHID(phy, 0, 0, vendor_id, product_id, product_release)
{
    //_lock_status = 0;
    // User or child responsible for calling connect or init
}

PicoGamepad::~PicoGamepad()
{
}

const uint8_t *PicoGamepad::report_desc()
{
    static const uint8_t reportDescriptor[] = {
        0x05, 0x01, // USAGE_PAGE (Generic Desktop)
        0x09, 0x04, // USAGE (Gamepad)
        0xa1, 0x01, // COLLECTION (Application)
        0x85, 0x01, //   REPORT_ID (1)

        0x05, 0x09, // USAGE_PAGE (Button)
        0x19, 0x01, // USAGE_MINIMUM (Button 1)
        0x29, 0x02, // USAGE_MAXIMUM (Button 2)
        0x15, 0x00, // LOGICAL_MINIMUM (0)
        0x25, 0x01, // LOGICAL_MAXIMUM (1)
        0x95, 0x02, // REPORT_COUNT (2)
        0x75, 0x01, // REPORT_SIZE (1)
        0x81, 0x02, // INPUT (Data,Var,Abs)

        0xc0 // END_COLLECTION

    };
    reportLength = sizeof(reportDescriptor);
    return reportDescriptor;
}

void PicoGamepad::SetButton(int idx, bool val)
{
    if (idx > 1 || idx < 0)
    {
        return;
    }
    bitWrite(inputData, idx, val);
}

bool PicoGamepad::send_update()
{
    _mutex.lock();

    HID_REPORT report;
    report.data[0] = 0x01;
    report.data[1] = inputData;
    report.length = 2;

    if (!send(&report))
    {
        _mutex.unlock();
        return false;
    }

    _mutex.unlock();
    return true;
}

#define DEFAULT_CONFIGURATION (1)
#define TOTAL_DESCRIPTOR_LENGTH ((1 * CONFIGURATION_DESCRIPTOR_LENGTH) + (1 * INTERFACE_DESCRIPTOR_LENGTH) + (1 * HID_DESCRIPTOR_LENGTH) + (2 * ENDPOINT_DESCRIPTOR_LENGTH))

const uint8_t *PicoGamepad::configuration_desc(uint8_t index)
{
    if (index != 0)
    {
        return NULL;
    }
    uint8_t configuration_descriptor_temp[] = {
        CONFIGURATION_DESCRIPTOR_LENGTH, // bLength
        CONFIGURATION_DESCRIPTOR,        // bDescriptorType
        LSB(TOTAL_DESCRIPTOR_LENGTH),    // wTotalLength (LSB)
        MSB(TOTAL_DESCRIPTOR_LENGTH),    // wTotalLength (MSB)
        0x01,                            // bNumInterfaces
        DEFAULT_CONFIGURATION,           // bConfigurationValue
        0x00,                            // iConfiguration
        C_RESERVED | C_SELF_POWERED,     // bmAttributes
        C_POWER(0),                      // bMaxPower

        INTERFACE_DESCRIPTOR_LENGTH, // bLength
        INTERFACE_DESCRIPTOR,        // bDescriptorType
        0x00,                        // bInterfaceNumber
        0x00,                        // bAlternateSetting
        0x02,                        // bNumEndpoints
        HID_CLASS,                   // bInterfaceClass
        HID_SUBCLASS_BOOT,           // bInterfaceSubClass
        HID_PROTOCOL_KEYBOARD,       // bInterfaceProtocol
        0x00,                        // iInterface

        HID_DESCRIPTOR_LENGTH,                // bLength
        HID_DESCRIPTOR,                       // bDescriptorType
        LSB(HID_VERSION_1_11),                // bcdHID (LSB)
        MSB(HID_VERSION_1_11),                // bcdHID (MSB)
        0x00,                                 // bCountryCode
        0x01,                                 // bNumDescriptors
        REPORT_DESCRIPTOR,                    // bDescriptorType
        (uint8_t)(LSB(report_desc_length())), // wDescriptorLength (LSB)
        (uint8_t)(MSB(report_desc_length())), // wDescriptorLength (MSB)

        ENDPOINT_DESCRIPTOR_LENGTH, // bLength
        ENDPOINT_DESCRIPTOR,        // bDescriptorType
        _int_in,                    // bEndpointAddress
        E_INTERRUPT,                // bmAttributes
        LSB(MAX_HID_REPORT_SIZE),   // wMaxPacketSize (LSB)
        MSB(MAX_HID_REPORT_SIZE),   // wMaxPacketSize (MSB)
        1,                          // bInterval (milliseconds)

        ENDPOINT_DESCRIPTOR_LENGTH, // bLength
        ENDPOINT_DESCRIPTOR,        // bDescriptorType
        _int_out,                   // bEndpointAddress
        E_INTERRUPT,                // bmAttributes
        LSB(MAX_HID_REPORT_SIZE),   // wMaxPacketSize (LSB)
        MSB(MAX_HID_REPORT_SIZE),   // wMaxPacketSize (MSB)
        1,                          // bInterval (milliseconds)
    };
    MBED_ASSERT(sizeof(configuration_descriptor_temp) == sizeof(_configuration_descriptor));
    memcpy(_configuration_descriptor, configuration_descriptor_temp, sizeof(_configuration_descriptor));
    return _configuration_descriptor;
}
