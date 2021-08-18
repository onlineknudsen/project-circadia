#include "Hardware/Display.h"
#include "Hardware/NarrowNumberFont.h"

Display::Display(int csPin) : mx_(HARDWARE_TYPE, csPin, MAX_DEVICES) {
    mx_.begin();
    mx_.setFont(_narrow_num_single);
    mx_.control(MD_MAX72XX::INTENSITY, 0);
    memset(displayBuffer_, 0, DISPLAY_WIDTH);
}

void Display::printMsgCenterUnderline(const char* msg, byte startChar, byte endChar) {
    byte length = strlen(msg);

    byte charBuffer[8] = { 0 };
    byte rawBuffer[DISPLAY_WIDTH] = { 0 };

    byte msgWidth = 0;

    byte startCol = 0;
    byte endCol = 0;
    for(byte i = 0; i < length; i++) {
        if(i == startChar) {
            startCol = msgWidth;
        }
        byte charWidth = mx_.getChar(msg[i], 8, charBuffer);
        memcpy(rawBuffer + msgWidth, charBuffer, charWidth);
        msgWidth += charWidth + CHAR_SPACING;
        if(i == endChar) {
            endCol = msgWidth - 1;
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

void Display::underline(byte startCol, byte endCol) {
    for(byte i = startCol; i < endCol; i++) {
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

void Display::invert() {
    for(byte col = 0; col < DISPLAY_WIDTH; col++) {
        displayBuffer_[col] = ~displayBuffer_[col];
    }
}

void Display::setScrollMsg(char* msg) {
    memset(scrollBuffer_, 0, sizeof scrollBuffer_);
    byte length = strlen(msg);
    byte charBuffer[8] = { 0 };

    uint16_t msgWidth = 0;
    for(byte i = 0; i < length; i++) {
        byte charWidth = mx_.getChar(msg[i], 8, charBuffer);
        memcpy(scrollBuffer_ + msgWidth, charBuffer, charWidth);
        msgWidth += charWidth + CHAR_SPACING;
    }

    scrollMsgLength_ = msgWidth;
    currCol_ = -DISPLAY_WIDTH;
}

// Optimize? I guess memcpy's are cheap but it feels like it's being unnecessary overwritten
void Display::displayScrollLeft() {
    if (currCol_ < 0) {
        memcpy(displayBuffer_ - currCol_, scrollBuffer_, DISPLAY_WIDTH + currCol_);
    }
    else if (currCol_ + DISPLAY_WIDTH > SCROLL_WIDTH) {
        memcpy(displayBuffer_, scrollBuffer_, SCROLL_WIDTH - 1 - currCol_);
    }
    else {
        memcpy(displayBuffer_, scrollBuffer_ + currCol_, DISPLAY_WIDTH);
        if(currCol_ >= scrollMsgLength_ - (DISPLAY_WIDTH / 2)) {
            int loopStartCol = currCol_ - scrollMsgLength_ - (DISPLAY_WIDTH / 2);
            memcpy(displayBuffer_ - loopStartCol, scrollBuffer_, DISPLAY_WIDTH + loopStartCol);
        }
    }

    unsigned long now = millis();
    if(now - lastScroll_ > scrollTime_) {
        currCol_++;

        if(currCol_ > scrollMsgLength_) {
            currCol_ = -DISPLAY_WIDTH / 2;
        }
        lastScroll_ = now;
    }
}

void Display::debugDisplay() {
    Serial.println();
    for(byte bit = 0; bit < 8; bit++) {
        for(byte col = 0; col < DISPLAY_WIDTH; col++) {
            Serial.print(((displayBuffer_[col] >> bit) & 1) ? "█" : " ");
        }
        Serial.println();
    }
}