/*
* light weight WS2812 lib V2.1 - Arduino support
*
* Controls WS2811/WS2812/WS2812B RGB-LEDs
* Author: Matthias Riegler
*
* Mar 07 2014: Added Arduino and C++ Library
*
* September 6, 2014:	Added option to switch between most popular color orders
*						(RGB, GRB, and BRG) --  Windell H. Oskay
*
* License: GNU GPL v2 (see License.txt)
*/

#include "WS2812.h"
#include <stdlib.h>
#include <string.h>

WS2812::WS2812(uint16_t num_leds) {
	count_led = num_leds;

	pixels = (uint8_t*)malloc(count_led * 3);
	memset(pixels, 0, count_led * 3);
	#ifdef RGB_ORDER_ON_RUNTIME
		offsetGreen = 0;
		offsetRed = 1;
		offsetBlue = 2;
	#endif
}

cRGB WS2812::getColorAt(uint16_t index) {

	cRGB px_value;

	if(index < count_led) {

		uint16_t tmp;
		tmp = index * 3;

		px_value.r = pixels[OFFSET_R(tmp)];
		px_value.g = pixels[OFFSET_G(tmp)];
		px_value.b = pixels[OFFSET_B(tmp)];
	}

	return px_value;
}

void WS2812::fillColor(cRGB px_value) {
	for(uint16_t index = 0; index < count_led; index++) {
		setColorAt(index, px_value);
	}
}

void WS2812::pushColor(cRGB px_value) {
	for(uint16_t index = count_led-1; index > 0; index--) {

		uint16_t copy_from = (index - 1) * 3;
		uint16_t copy_to = index * 3;

		pixels[OFFSET_R(copy_to)] = pixels[OFFSET_R(copy_from)];
		pixels[OFFSET_G(copy_to)] = pixels[OFFSET_G(copy_from)];
		pixels[OFFSET_B(copy_to)] = pixels[OFFSET_B(copy_from)];
	}

	setColorAt(0, px_value);
}


void WS2812::setColorAt(uint16_t index, cRGB px_value) {
	if(index < count_led) {

		uint16_t tmp;
		tmp = index * 3;

		pixels[OFFSET_R(tmp)] = px_value.r;
		pixels[OFFSET_G(tmp)] = px_value.g;
		pixels[OFFSET_B(tmp)] = px_value.b;
	}
}

void WS2812::setSubpixelAt(uint16_t index, uint8_t offset, uint8_t px_value) {
	if (index < count_led) {
		uint16_t tmp;
		tmp = index * 3;

		pixels[tmp + offset] = px_value;
	}
}

void WS2812::sync() {
	*ws2812_port_reg |= pinMask; // Enable DDR
	lowlevelSend(pixels, 3*count_led, pinMask,(uint8_t*) ws2812_port);
}

#ifdef RGB_ORDER_ON_RUNTIME
void WS2812::setColorOrderGRB() { // Default color order
	offsetGreen = 0;
	offsetRed = 1;
	offsetBlue = 2;
}

void WS2812::setColorOrderRGB() {
	offsetRed = 0;
	offsetGreen = 1;
	offsetBlue = 2;
}

void WS2812::setColorOrderBRG() {
	offsetBlue = 0;
	offsetRed = 1;
	offsetGreen = 2;
}
#endif

WS2812::~WS2812() {
	free(pixels);
}

#ifndef ARDUINO
void WS2812::setOutput(const volatile uint8_t* port, volatile uint8_t* reg, uint8_t pin) {
	pinMask = (1<<pin);
	ws2812_port = port;
	ws2812_port_reg = reg;
}
#else
void WS2812::setOutput(uint8_t pin) {
	pinMask = digitalPinToBitMask(pin);
	ws2812_port = portOutputRegister(digitalPinToPort(pin));
	ws2812_port_reg = portModeRegister(digitalPinToPort(pin));
}
#endif
