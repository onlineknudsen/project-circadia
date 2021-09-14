#include "Hardware/SDAudioSource.h"

SDAudioSource::SDAudioSource(int sdCS, int sdBufferSize, int i2sBClk, int i2sLRC, int i2sDataOut) {
    sdBufferSize_ = sdBufferSize;
    sdBuffer_ = new byte[sdBufferSize_];

    initSD(sdCS);
    initI2S(i2sBClk, i2sLRC, i2sDataOut);
    loadSongTitles();
}

SDAudioSource::~SDAudioSource() {
    i2s_driver_uninstall(I2S_PORT);
    if(audioFile_)
        audioFile_.close();
    delete sdBuffer_;
}

void SDAudioSource::setCurrentSong(int index) {
    if(index > songCount_ || index < 0) {
        Serial.println("setCurrentSong: Index out of bounds");
        return;
    }

    currentSongIndex_ = index;
}

void SDAudioSource::loadCurrentSong() {
    char title[3];
    if(currentSongIndex_ == songCount_) {
        int randomIndex = random(songCount_);
        itoa(randomIndex, title, 10);
    } else {
        itoa(currentSongIndex_, title, 10);
    }

    setAudio(title);
}

void SDAudioSource::setAudio(const char* name) {
    if(audioFile_)
        audioFile_.close();

    const int pathStrLength = strlen(audioDir) + strlen(name) + strlen(".wav");
    char path[pathStrLength];

    sprintf(path, "%s%s.wav", audioDir, name);
    audioFile_ = SD.open(path);
    if(!audioFile_) {
        Serial.print("No audio file found at ");
        Serial.println(path);
        return;
    }
    audioFile_.seek(44);
    memset(sdBuffer_, 0, sdBufferSize_);
}

bool SDAudioSource::play() {
    if(!audioEnabled_) {
        audioEnabled_ = true;
        i2s_start(I2S_PORT);
    }

    if(audioFile_.available() > 0) {
        audioFile_.read(sdBuffer_, sdBufferSize_);
        unsigned int writtenBytes;
        i2s_write(I2S_PORT, sdBuffer_, sdBufferSize_, &writtenBytes, portMAX_DELAY);
        return true;
    } else {
        i2s_zero_dma_buffer(I2S_PORT);
        return false;
    }

}

void SDAudioSource::loop() {
    bool isDone = !play();
    if(isDone) {
        restart();
    }
}

void SDAudioSource::restart() {
    audioFile_.seek(44);
}

void SDAudioSource::stop() {
    audioEnabled_ = false;
    i2s_zero_dma_buffer(I2S_PORT);
    i2s_stop(I2S_PORT);
}

void SDAudioSource::getSongTitle(int index, char* title, size_t size) {
    if(index > songCount_ - 1 || index < 0) {
        Serial.println("getSongTitle: Index out of bounds");
        return;
    }

    if(size < strlen(songTitles_[index])) {
        Serial.println("Not enough space allocated for song title");
        return;
    }

    strcpy(title, songTitles_[index]);
}

void SDAudioSource::getCurrentSongTitle(char* title, size_t size) {
    getSongTitle(currentSongIndex_, title, size);
}

int SDAudioSource::getCurrentSongIndex() {
    return currentSongIndex_;
}

byte SDAudioSource::getSongCount() {
    return songCount_;
}

void SDAudioSource::loadSongTitles() {
    if(audioFile_) {
        audioFile_.close();
    }

    const int pathStrLength = strlen(audioDir) + strlen("songs.txt");
    char path[pathStrLength];
    sprintf(path, "%ssongs.txt", audioDir);

    File songTitleFile_ = SD.open(path);

    while(songTitleFile_.available()) {
        songTitleFile_.readBytesUntil('\n', songTitles_[songCount_], MAX_SONG_TITLE_LENGTH);
        songCount_++;
    }
    songTitleFile_.close();
}

void SDAudioSource::initSD(int sdCS) {
    if(!SD.begin(sdCS)) {
        Serial.println("SD card could not be initialized");
        while(1);
    }
}

void SDAudioSource::initI2S(int i2sBClk, int i2sLRC, int i2sDataOut) {
    i2s_config_t i2sConfig = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
        .sample_rate = SAMPLE_RATE,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
        .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
        .intr_alloc_flags = 0,
        .dma_buf_count = 16,
        .dma_buf_len = 64
    };

    i2s_pin_config_t pinConfig = {
        .bck_io_num = i2sBClk,
        .ws_io_num = i2sLRC,
        .data_out_num = i2sDataOut,
        .data_in_num = -1   //Not used
    };

    i2s_driver_install(I2S_PORT, &i2sConfig, 0, NULL);
    i2s_set_pin(I2S_PORT, &pinConfig);
    i2s_set_clk(I2S_PORT, SAMPLE_RATE, (i2s_bits_per_sample_t)16, (i2s_channel_t)1);
    i2s_zero_dma_buffer(I2S_PORT);
}