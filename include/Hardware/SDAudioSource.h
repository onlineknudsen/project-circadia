#ifndef SD_AUDIO_SOURCE_H
#define SD_AUDIO_SOURCE_H

#include<Arduino.h>
#include<SD.h>

#include "I2S.h"

#define SD_BUFFER_SIZE 2400

#define MAX_SONG_TITLE_LENGTH 75
#define MAX_SONGS 20 // I may want to adjust this???

class SDAudioSource {
    public:
        SDAudioSource(int sdCS, int sdBufferSize = SD_BUFFER_SIZE, int i2sBClk = I2S_BCLK, int i2sLRC = I2S_LRC, int i2sDataOut = I2S_DOUT);
        ~SDAudioSource();
        void setCurrentSong(int index);
        void setAudio(const char* name);
        void loop();
        bool play();
        void stop();
        void restart();

        void getSongTitle(int index, char* title, size_t size);
        void getCurrentSongTitle(char* title, size_t size);

        int getCurrentSongIndex();

    private:
        void initSD(int sdCS);
        void initI2S(int i2sBClk, int i2sLRC, int i2sDataOut);

        void loadSongTitles();

        char songTitles_[MAX_SONGS][MAX_SONG_TITLE_LENGTH];
        byte songCount_ = 0;

        int currentSongIndex_ = 0;

        const char* audioDir = "/music/";
        File audioFile_;
        byte* sdBuffer_;
        int sdBufferSize_;
        bool audioEnabled_ = true;



};
#endif