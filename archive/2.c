// enumerate all input source
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include <stdio.h>

int main()
{
  
  ma_result result;
  ma_context context;
  ma_device_info* pCaptureInfos;
  ma_uint32 captureCount;
  ma_uint32 i;

  // init the context
  result = ma_context_init(NULL, 0, NULL, &context);
  if (result != MA_SUCCESS) {
    printf("failed to init context.\n");
    return -1;
  }

  // enumerate captured devices
  result = ma_context_get_devices(&context, NULL, NULL, &pCaptureInfos, &captureCount);
  if (result != MA_SUCCESS) {
    printf("failed to retrive capture devices.\n");
    return -1;
  }

  // print list of input devices
  printf("capture devices: \n");
  for (i=0; i<captureCount; i++) {
    printf(" %u: %s\n", i, pCaptureInfos[i].name);
  }

  ma_context_uninit(&context);
  
  return 0;
}
