#ifndef DISPLAY_H
#define DISPLAY_H

#include<MD_MAX72xx.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4

#define DISPLAY_WIDTH MAX_DEVICES * COL_SIZE
#define CHAR_SPACING 1

#define SCROLL_WIDTH DISPLAY_WIDTH * 20 // HOW MUCH MEMORY IS TOO MUCH MEMORY? 32 * 20 = 640 bytes

class Display {
    public:
        void printMsgCenter(const char* msg);
        void printMsgCenterUnderline(const char* msg, byte startChar, byte endChar);

        void underline(byte startCol, byte endCol);
        void noUnderline();

        void invert();

        void clear();

        void setScrollMsg(char* msg);
        void displayScrollLeft();

        Display(int csPin);
        void update();

    private:

        MD_MAX72XX mx_;
        byte displayBuffer_[DISPLAY_WIDTH];
        byte currentDisplayBuffer_[DISPLAY_WIDTH];

        byte scrollBuffer_[SCROLL_WIDTH];

        int scrollMsgLength_;
        int scrollTime_ = 50; // TODO: Customizable scroll time, based on length of message (long messages faster)
        unsigned long lastScroll_;

        int currCol_ = -DISPLAY_WIDTH;

        void debugDisplay();
};
#endif