#include "Hardware/Display.h"
#include "Hardware/NarrowNumberFont.h"

Display::Display(int csPin) : mx_(HARDWARE_TYPE, csPin, MAX_DEVICES) {
    mx_.begin();
    mx_.setFont(_narrow_num_single);
    mx_.control(MD_MAX72XX::INTENSITY, 0);
    memset(displayBuffer_, 0, DISPLAY_WIDTH);
}

void Display::printMsgCenter(const char* msg) {
   byte length = strlen(msg);
   byte charBuffer[8] = { 0 };
   byte rawBuffer[DISPLAY_WIDTH] = { 0 };

   byte msgWidth = 0;
   for(byte i = 0; i < length; i++) {
       byte charWidth = mx_.getChar(msg[i], 8, charBuffer);
       memcpy(rawBuffer + msgWidth, charBuffer, charWidth);
       msgWidth += charWidth + CHAR_SPACING;
   }

   byte padding = (DISPLAY_WIDTH - msgWidth + 1) / 2;
   memcpy(displayBuffer_ + padding, rawBuffer, msgWidth);
}

void Display::update() {
    if(memcmp(displayBuffer_, currentDisplayBuffer_, DISPLAY_WIDTH) != 0) {
        mx_.control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);
        for(byte col = 0; col < DISPLAY_WIDTH; col++) {
            byte colData = displayBuffer_[DISPLAY_WIDTH - 1 - col];
            mx_.setColumn(col, colData);
        }
        mx_.control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
        memcpy(currentDisplayBuffer_, displayBuffer_, DISPLAY_WIDTH);
    }
    memset(displayBuffer_, 0, DISPLAY_WIDTH);
}

void Display::clear() {
    memset(displayBuffer_, 0, DISPLAY_WIDTH);
}