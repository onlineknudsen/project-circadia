#ifndef DISPLAY_H
#define DISPLAY_H

#include<MD_MAX72xx.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4

#define DISPLAY_WIDTH MAX_DEVICES * COL_SIZE
#define CHAR_SPACING 1

class Display {
    public:
        void printMsgCenter(const char* msg);
        void printMsgCenterUnderline(const char* msg, int startChar, int endChar);
        void underline(int startCol, int endCol);
        void noUnderline();
        void clear();
        Display(int csPin);
        void update();

    private:

        MD_MAX72XX mx_;
        byte displayBuffer_[DISPLAY_WIDTH];
        byte currentDisplayBuffer_[DISPLAY_WIDTH];
};
#endif