#pragma once

#include "_NDL_directmedia_types.h"

#define NDL_DIRECTMEDIA_API_VERSION 1

enum NDL_DIRECTAUDIO_CH_
{
    NDL_DIRECTAUDIO_CH_MAIN = 0,
    NDL_DIRECTAUDIO_CH_SUB = 1
};
typedef enum NDL_DIRECTAUDIO_CH_ NDL_DIRECTAUDIO_CH_T;

enum NDL_DIRECTAUDIO_SRC_TYPE_
{
    /** PCM Mix Channel */
    NDL_DIRECTAUDIO_SRC_TYPE_PCM = 1,
    NDL_DIRECTAUDIO_SRC_TYPE_AC3 = 2,
    /** AAC. audio/mpeg,mpegversion=4,stream-format=adts */
    NDL_DIRECTAUDIO_SRC_TYPE_AAC = 3,
    NDL_DIRECTAUDIO_SRC_TYPE_WTF = 4
};
typedef enum NDL_DIRECTAUDIO_SRC_TYPE_ NDL_DIRECTAUDIO_SRC_TYPE_T;

typedef struct
{
    unsigned int numChannel;
    unsigned int bitPerSample;
    /**
     * @brief Nodelay setting - On(1)/Off(0)
     */
    unsigned int nodelay;
    unsigned int upperThreshold;
    unsigned int lowerThreshold;
    NDL_DIRECTAUDIO_CH_T channel;
    NDL_DIRECTAUDIO_SRC_TYPE_T srcType;
    NDL_DIRECTAUDIO_SAMPLING_FREQ_T samplingFreq;
} NDL_DIRECTAUDIO_DATA_INFO;

typedef struct
{
    int width;
    int height;
} NDL_DIRECTVIDEO_DATA_INFO;

typedef void (*NDLVideoPlayCallback)(unsigned long long);

int NDL_DirectAudioOpen(NDL_DIRECTAUDIO_DATA_INFO *info);
int NDL_DirectAudioClose();

/**
 * @return 0 if succeeded
 */
int NDL_DirectAudioPlay(const void *data, size_t size);

/**
 * @return 0 if succeeded
 */
int NDL_DirectAudioPause();

int NDL_DirectAudioResume();

int NDL_DirectAudioCheckBufferSize(unsigned int *size);

int NDL_DirectVideoOpen(NDL_DIRECTVIDEO_DATA_INFO *info);
int NDL_DirectVideoClose();

int NDL_DirectVideoSetCallback(NDLVideoPlayCallback cb);

/**
 * @return 0 if succeeded
 */
int NDL_DirectVideoPlay(const void *data, size_t size);
/**
 * @return 0 if succeeded
 */
int NDL_DirectVideoPlayWithCallback(const void *data, size_t size, unsigned long long userdata);
int NDL_DirectVideoStop();