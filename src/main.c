#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include <stdio.h>
#include <stdlib.h> // For malloc

#ifdef __EMSCRIPTEN__
void main_loop__em()
{
}
#endif

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    MA_ASSERT(pDevice->capture.format == pDevice->playback.format);
    MA_ASSERT(pDevice->capture.channels == pDevice->playback.channels);

    /* In this example the format and channel count are the same for both input and output which means we can just memcpy(). */
    MA_COPY_MEMORY(pOutput, pInput, frameCount * ma_get_bytes_per_frame(pDevice->capture.format, pDevice->capture.channels));
}

void clear_input_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

int main(int argc, char** argv)
{
    ma_result result;
    ma_context context;
    ma_device_info* pCaptureInfos;
    ma_device_info* pPlaybackInfos;
    ma_uint32 captureCount;
    ma_uint32 playbackCount;
    ma_uint32 i;
    ma_device_id captureDeviceId;
    ma_device_id playbackDeviceId;
    int selectedCaptureDeviceIndex;
    int selectedPlaybackDeviceIndex;

    // Init the context
    result = ma_context_init(NULL, 0, NULL, &context);
    if (result != MA_SUCCESS) {
        printf("Failed to init context.\n");
        return -1;
    }

    // Enumerate capture and playback devices
    result = ma_context_get_devices(&context, &pPlaybackInfos, &playbackCount, &pCaptureInfos, &captureCount);
    if (result != MA_SUCCESS) {
        printf("Failed to retrieve devices.\n");
        ma_context_uninit(&context);
        return -1;
    }

    // Print list of capture devices
    printf("Capture devices: \n");
    for (i = 0; i < captureCount; i++) {
        printf(" %u: %s\n", i, pCaptureInfos[i].name);
    }

    // Prompt user to select a capture device
    printf("Select a capture device by index: ");
    if (scanf_s("%d", &selectedCaptureDeviceIndex) != 1 || selectedCaptureDeviceIndex < 0 || (ma_uint32)selectedCaptureDeviceIndex >= captureCount) {
        printf("Invalid device index.\n");
        ma_context_uninit(&context);
        return -1;
    }
    clear_input_buffer(); // Clear the input buffer

    // Print list of playback devices
    printf("Playback devices: \n");
    for (i = 0; i < playbackCount; i++) {
        printf(" %u: %s\n", i, pPlaybackInfos[i].name);
    }

    // Prompt user to select a playback device
    printf("Select a playback device by index: ");
    if (scanf_s("%d", &selectedPlaybackDeviceIndex) != 1 || selectedPlaybackDeviceIndex < 0 || (ma_uint32)selectedPlaybackDeviceIndex >= playbackCount) {
        printf("Invalid device index.\n");
        ma_context_uninit(&context);
        return -1;
    }
    clear_input_buffer(); // Clear the input buffer

    // Initialize the selected capture and playback devices
    captureDeviceId = pCaptureInfos[selectedCaptureDeviceIndex].id;
    playbackDeviceId = pPlaybackInfos[selectedPlaybackDeviceIndex].id;

    ma_device_config deviceConfig;
    ma_device device;

    deviceConfig = ma_device_config_init(ma_device_type_duplex);
    deviceConfig.capture.pDeviceID  = &captureDeviceId;
    deviceConfig.capture.format     = ma_format_s16;
    deviceConfig.capture.channels   = 2;
    deviceConfig.capture.shareMode  = ma_share_mode_shared;
    deviceConfig.playback.pDeviceID = &playbackDeviceId;
    deviceConfig.playback.format    = ma_format_s16;
    deviceConfig.playback.channels  = 2;
    deviceConfig.dataCallback       = data_callback;

    result = ma_device_init(&context, &deviceConfig, &device);
    if (result != MA_SUCCESS) {
        printf("Failed to initialize device.\n");
        ma_context_uninit(&context);
        return -1;
    }

    result = ma_device_start(&device);
    if (result != MA_SUCCESS) {
        printf("Failed to start device.\n");
        ma_device_uninit(&device);
        ma_context_uninit(&context);
        return -1;
    }

    printf("Press Enter to quit...\n");
    getchar();

    ma_device_uninit(&device);
    ma_context_uninit(&context);

    return 0;
}

