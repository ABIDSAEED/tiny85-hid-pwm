/**
 * main.c 
 */

#include <avr/io.h>
#include <avr/interrupt.h>
//#include <avr/eeprom.h>
#include <util/delay.h>

#include "usbdrv.h"

//static uint8_t EEPROMOCR0B __attribute__((section(".eeprom")));

// HID report descriptor
//  1. OUTPUT(host->device), 8bit * 1
PROGMEM const char usbHidReportDescriptor[USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH] = {
    0x06, 0x00, 0xff,   // USAGE_PAGE (Vendor Defined Page 1)
    0x09, 0x01,         // USAGE (Vendor Usage 1)
    0x15, 0x00,         // LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,   // LOGICAL_MAXIMUM (255)
    0xa1, 0x01,         // COLLECTION (Application)
        
    0x75, 0x08,         // REPORT_SIZE (8)bit
    0x95, 0x01,         // REPORT_COUNT (1)
    0x09, 0x00,         // USAGE (Undefined)
    0x91, 0x02,         // OUTPUT (Data,Var,Abs)
    
    0xc0                // END_COLLECTION
};

// host -> device, receive data
uchar usbFunctionWrite(uchar *data, uchar len)
{
    if (len > 0)
    {
        OCR0B = data[0];// set pulse width
        //eeprom_write_byte(&EEPROMOCR0B, data[0]);
    }
    
    return len;
}

// handle request
usbMsgLen_t usbFunctionSetup(uchar data[8])
{
    usbRequest_t    *rq = (void *)data;

    if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS)
    {
        // host -> device
        if (rq->bRequest == USBRQ_HID_SET_REPORT)
        {
            return USB_NO_MSG;
        }
    }
    return 0;
}

// main
void deviceInitialize(void)
{
    // gpio
    DDRB |= _BV(DDB1);
    
    // pwm generator; output to OC0B(PB1,No6), wgm = 111
    TCCR0A |= _BV(COM0B1) | _BV(WGM01) | _BV(WGM00);
    TCCR0B |= _BV(WGM02) | _BV(CS00);// clk/1; (16.5MHz/1)/OCR0A = 64.7kHz
    
    //OCR0A = eeprom_read_byte(&EEPROMOCR0B);
    OCR0A = 255;  // top value; period
    OCR0B = 255;  // compare value; duty 8bit
    
    // reset usb driver
    usbInit();
    usbDeviceDisconnect();

    _delay_ms(500);
    usbDeviceConnect();
    sei();
}

int main(void)
{
    deviceInitialize();
    
    while (1)
    {
        usbPoll();
    }
}
