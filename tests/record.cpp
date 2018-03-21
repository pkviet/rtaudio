/******************************************/
/*
  record.cpp
  by Gary P. Scavone, 2007

  This program records audio from a device and writes it to a
  header-less binary file.  Use the 'playraw', with the same
  parameters and format settings, to playback the audio.
*/
/******************************************/

#include "RtAudio.h"
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <stdio.h>
#include <thread>

/*
typedef char MY_TYPE;
#define FORMAT RTAUDIO_SINT8
*/

typedef signed short MY_TYPE;
#define FORMAT RTAUDIO_SINT24

/*
typedef S24 MY_TYPE;
#define FORMAT RTAUDIO_SINT24

typedef signed long MY_TYPE;
#define FORMAT RTAUDIO_SINT32

typedef float MY_TYPE;
#define FORMAT RTAUDIO_FLOAT32

typedef double MY_TYPE;
#define FORMAT RTAUDIO_FLOAT64
*/

// Platform-dependent sleep routines.
#if defined( __WINDOWS_ASIO__ ) || defined( __WINDOWS_DS__ ) || defined( __WINDOWS_WASAPI__ )
  #include <windows.h>
  #define SLEEP( milliseconds ) Sleep( (DWORD) milliseconds ) 
#else // Unix variants
  #include <unistd.h>
  #define SLEEP( milliseconds ) usleep( (unsigned long) (milliseconds * 1000.0) )
#endif

void usage( void ) {
  // Error function in case of incorrect command-line
  // argument specifications
  std::cout << "\nuseage: record N fs <duration> <device> <channelOffset>\n";
  std::cout << "    where N = number of channels,\n";
  std::cout << "    fs = the sample rate,\n";
  std::cout << "    duration = optional time in seconds to record (default = 2.0),\n";
  std::cout << "    device = optional device to use (default = 0),\n";
  std::cout << "    and channelOffset = an optional channel offset on the device (default = 0).\n\n";
  exit( 0 );
}

struct InputData {
  MY_TYPE* buffer;
  unsigned long bufferBytes;
  unsigned long totalFrames;
  unsigned long frameCounter;
  unsigned int channels;
};

// Interleaved buffers
int input( void * /*outputBuffer*/, void *inputBuffer, unsigned int nBufferFrames,
           double /*streamTime*/, RtAudioStreamStatus /*status*/, void *data )
{
  InputData *iData = (InputData *) data;

  // Simply copy the data to our allocated buffer.
  unsigned int frames = nBufferFrames;
  if ( iData->frameCounter + nBufferFrames > iData->totalFrames ) {
    frames = iData->totalFrames - iData->frameCounter;
    iData->bufferBytes = frames * iData->channels * sizeof( MY_TYPE );
  }

  unsigned long offset = iData->frameCounter * iData->channels;
  memcpy( iData->buffer+offset, inputBuffer, iData->bufferBytes );
  iData->frameCounter += frames;

  if ( iData->frameCounter >= iData->totalFrames ) return 2;
  return 0;
}

int main( int argc/*, char *argv[]*/ )
{
  unsigned int channels, fs, bufferFrames, bufferFramesbis, device = 0, devicebis, offset = 0;
  unsigned int i, j;
  double time = 2.0;
  FILE *fd;
  std::thread t[2];

  // minimal command-line checking
  // if ( argc < 3 || argc > 6 ) usage();

  channels = 4;
  fs = 44100;
  time = (double)10;
  device = (unsigned int)4;
  devicebis = (unsigned int)5;
  offset = (unsigned int)0;

  RtAudio adc, adcbis;
  if ( adc.getDeviceCount() < 1 ) {
    std::cout << "\nNo audio devices found!\n";
    exit( 1 );
  }
  std::cout << "number of devices " << adc.getDeviceCount() << "\n";

  // Scan through devices for various capabilities
  RtAudio::DeviceInfo info;
  // Determine the number of devices available
  unsigned int nb_devices = adc.getDeviceCount();
  for (i = 0;  i < nb_devices; i++) {
	  info = adc.getDeviceInfo(i);
	  if (info.probed == true) {
		  // Print, for example, the maximum number of output channels for each device
		  std::cout << "device = " << i;
		  std::cout << "\n maximum input channels = " << info.inputChannels << "\n";
		  std::cout << " maximum output channels = " << info.outputChannels << "\n";
		  std::cout << " preferred sample rate = " << info.preferredSampleRate << "\n";
		  std::cout << " native formats (bitmask) = " << info.nativeFormats << "\n";
		  std::cout << " preferred sample rate = " << info.preferredSampleRate << "\n";
		  size_t size = info.sampleRates.size();
		  for (j = 0; j < size; j++) {
			  std::cout << " available sample rate = " << info.sampleRates[j] << "\n";
		  }
		  std::cout << "probed is = " << info.probed << "\n";
	  }
  }



  // Let RtAudio print messages to stderr.
  adc.showWarnings( true );
  adcbis.showWarnings(true);

  //show info on the devices

  // Set our stream parameters for input only.
  bufferFrames = 512;
  RtAudio::StreamParameters iParams, iParamsbis;
 
  iParams.deviceId = device;
  iParams.nChannels = channels;
  iParams.firstChannel = offset;

  iParamsbis.deviceId = devicebis;
  iParamsbis.nChannels = channels;
  iParamsbis.firstChannel = offset;

  InputData data, databis;
  data.buffer = 0;
  databis.buffer = 0;

  try {
    adc.openStream( NULL, &iParams, FORMAT, fs, &bufferFrames, &input, (void *)&data );
  }
  catch ( RtAudioError& e ) {
    std::cout << '\n' << e.getMessage() << '\n' << std::endl;
    goto cleanup;
  }

  try {
	  adcbis.openStream(NULL, &iParamsbis, FORMAT, fs, &bufferFramesbis, &input, (void *)&databis);
  }
  catch (RtAudioError& ebis) {
	  std::cout << '\n' << ebis.getMessage() << '\n' << std::endl;
	  goto cleanup;
  }

  data.bufferBytes = bufferFrames * channels * sizeof( MY_TYPE );
  data.totalFrames = (unsigned long) (fs * time);
  data.frameCounter = 0;
  data.channels = channels;

  databis.bufferBytes = bufferFramesbis * channels * sizeof(MY_TYPE);
  databis.totalFrames = (unsigned long)(fs * time);
  databis.frameCounter = 0;
  databis.channels = channels;

  unsigned long totalBytes, totalBytesbis;
  totalBytes = data.totalFrames * channels * sizeof( MY_TYPE );
  totalBytesbis = databis.totalFrames * channels * sizeof(MY_TYPE);

  // Allocate the entire data buffer before starting stream.
  data.buffer = (MY_TYPE*) malloc( totalBytes );
  if ( data.buffer == 0 ) {
    std::cout << "Memory allocation error ... quitting!\n";
    goto cleanup;
  }
  databis.buffer = (MY_TYPE*)malloc(totalBytesbis);
  if (databis.buffer == 0) {
	  std::cout << "Memory allocation error ... quitting!\n";
	  goto cleanup;
  }

  try {
    adc.startStream();
  }
  catch ( RtAudioError& e ) {
    std::cout << '\n' << e.getMessage() << '\n' << std::endl;
    goto cleanup;
  }

  try {
	  adcbis.startStream();
  }
  catch (RtAudioError& e) {
	  std::cout << '\n' << e.getMessage() << '\n' << std::endl;
	  goto cleanup;
  }

  std::cout << "\nRecording for " << time << " seconds ... writing file 'record.raw' (buffer frames = " << bufferFrames << ")." << std::endl;
  while ( adc.isStreamRunning() ) {
    SLEEP( 100 ); // wake every 100 ms to check if we're done
  }
  while (adcbis.isStreamRunning()) {
	  SLEEP(100); // wake every 100 ms to check if we're done
  }

  // Now write the entire data to the file.
  fd = fopen( "record.raw", "wb" );
  fwrite( data.buffer, sizeof( MY_TYPE ), data.totalFrames * channels, fd );
  fclose( fd );

  fd = fopen("record2.raw", "wb");
  fwrite(databis.buffer, sizeof(MY_TYPE), databis.totalFrames * channels, fd);
  fclose(fd);

 cleanup:
  if ( adc.isStreamOpen() ) adc.closeStream();
  if ( data.buffer ) free( data.buffer );
  if (adcbis.isStreamOpen()) adcbis.closeStream();
  if (databis.buffer) free(databis.buffer);

  return 0;
}
