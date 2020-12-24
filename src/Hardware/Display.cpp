#include "Hardware/Display.h"
#include "Hardware/NarrowNumberFont.h"

Display::Display(int csPin) : mx_(HARDWARE_TYPE, csPin, MAX_DEVICES) {
    mx_.begin();
    mx_.setFont(_narrow_num_single);
    mx_.control(MD_MAX72XX::INTENSITY, 0);
    memset(displayBuffer_, 0, DISPLAY_WIDTH);
}

void Display::printMsgCenterUnderline(const char* msg, int startChar, int endChar) {
    byte length = strlen(msg);

    byte charBuffer[8] = { 0 };
    byte rawBuffer[DISPLAY_WIDTH] = { 0 };

    byte msgWidth = 0;

    int startCol = 0;
    int endCol = 0;
    for(int i = 0; i < length; i++) {
        if(i == startChar) {
            startCol = msgWidth - 1;
        }
        byte charWidth = mx_.getChar(msg[i], 8, charBuffer);
        memcpy(rawBuffer + msgWidth, charBuffer, charWidth);
        msgWidth += charWidth + CHAR_SPACING;
        if(i == endChar) {
            endCol = msgWidth;
        }
    }

    byte padding = (DISPLAY_WIDTH - msgWidth + 1) / 2;
    startCol += padding;
    if(startCol < 0) {
        startCol = 0;
    }

    endCol += padding;
    if(endCol > DISPLAY_WIDTH - 1) {
        endCol = DISPLAY_WIDTH - 1;
    }

    memcpy(displayBuffer_ + padding, rawBuffer, msgWidth);
    underline(startCol, endCol);
}

void Display::printMsgCenter(const char* msg) {
   byte length = strlen(msg);
   byte charBuffer[8] = { 0 };
   byte rawBuffer[DISPLAY_WIDTH] = { 0 };

   byte msgWidth = 0;
   for(int i = 0; i < length; i++) {
       byte charWidth = mx_.getChar(msg[i], 8, charBuffer);
       memcpy(rawBuffer + msgWidth, charBuffer, charWidth);
       msgWidth += charWidth + CHAR_SPACING;
   }

   byte padding = (DISPLAY_WIDTH - msgWidth + 1) / 2;
   memcpy(displayBuffer_ + padding, rawBuffer, msgWidth);
}

void Display::underline(int startCol, int endCol) {
    for(int i = startCol; i < endCol; i++) {
        displayBuffer_[i] |= 0b10000000;
    }
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