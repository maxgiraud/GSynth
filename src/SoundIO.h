#ifndef __SOUNDIO__
#define __SOUNDIO__

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <vector>
#include "portaudio.h"

#define SAMPLE_RATE   (44100)
#define FRAMES_PER_BUFFER  (64)

#ifndef M_PI
#define M_PI  (3.14159265)
#endif

#define TABLE_SIZE   (1000)
typedef struct
{
//    float sine[TABLE_SIZE];
    float* sine;
    int _size_;
    int left_phase;
    int right_phase;
    char message[20];
}
paTestData;



class SoundIO {

	private:

		PaStreamParameters outputParameters;
		PaStream* stream;
		PaError err;
		paTestData _data;
		int size;

	public:
		
		SoundIO();

		int Init();
		static void StreamFinished( void* userData );
		static int patestCallback(	const void *inputBuffer, void *outputBuffer,
						unsigned long framesPerBuffer,
						const PaStreamCallbackTimeInfo* timeInfo,
						PaStreamCallbackFlags statusFlags,
						void *userData );

		int On();
		int Off();
		int Play();
		int Pause();
		int Send(std::vector<double> data);
		int test();
};

#endif

