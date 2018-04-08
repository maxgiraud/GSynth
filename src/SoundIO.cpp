
#include "SoundIO.h"

using namespace std;

SoundIO::SoundIO () {

	Init();
}

int SoundIO::Init () {

	err = Pa_Initialize();
	if( err != paNoError ) goto error;

	outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
	if (outputParameters.device == paNoDevice) {
	  fprintf(stderr,"Error: No default output device.\n");
	  goto error;
	}

	outputParameters.channelCount = 2;       /* stereo output */
	outputParameters.sampleFormat = paFloat32; /* 32 bit floating point output */
	outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
	outputParameters.hostApiSpecificStreamInfo = NULL;

	return err;
error:
    Pa_Terminate();
    fprintf( stderr, "An error occured while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", err );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
    return err;
}


/*
 * This routine is called by portaudio when playback is done.
 */
/*static */void SoundIO::StreamFinished( void* userData )
{
   paTestData *_data = (paTestData *) userData;
   printf( "  |-- Stream Completed : %s\n", _data->message );
}


int SoundIO::patestCallback(	const void *inputBuffer, void *outputBuffer,
			    	unsigned long framesPerBuffer,
			    	const PaStreamCallbackTimeInfo* timeInfo,
			    	PaStreamCallbackFlags statusFlags,
			    	void *userData )
{
    paTestData *_data = (paTestData*)userData;
    float *out = (float*)outputBuffer;
    unsigned long i;

    (void) timeInfo; /* Prevent unused variable warnings. */
    (void) statusFlags;
    (void) inputBuffer;
    
//    for( i=0; i<framesPerBuffer; i++ )
//    {
//        *out++ = _buf[_data->left_phase]; 
//        *out++ = _buf[_data->right_phase]; 
//        _data->left_phase += 1;
//        if( _data->left_phase >= TABLE_SIZE ) _data->left_phase -= TABLE_SIZE;
//        _data->right_phase += 1; /* higher pitch so we can distinguish left and right. */
//        if( _data->right_phase >= TABLE_SIZE ) _data->right_phase -= TABLE_SIZE;
//    }
//cout << "READ - " << _data->_size_ << endl;
    for( i=0; i<framesPerBuffer; i++ )
    {
//cout << _data->left_phase << " / " << _data->_size_ << endl;
        *out++ = _data->sine[_data->left_phase];  /* left */
        *out++ = _data->sine[_data->right_phase];  /* right */
        _data->left_phase += 1;
        if( _data->left_phase >= _data->_size_ ) _data->left_phase -= _data->_size_;
        _data->right_phase += 1; /* higher pitch so we can distinguish left and right. */
        if( _data->right_phase >= _data->_size_ ) _data->right_phase -= _data->_size_;
    }
//cout << "OFF" << endl;
//cout << "caca" << endl;    
    return paContinue;
}


int SoundIO::On () {

    int i;

    printf("  |-- Enabling : PortAudio streaming, SR = %d, BufSize = %d\n", SAMPLE_RATE, FRAMES_PER_BUFFER);

    /* initialise sinusoidal wavetable */

_data.sine = new float[TABLE_SIZE];
    for( i=0; i<TABLE_SIZE; i++ )
    {
        _data.sine[i] = (float) sin( ((double)i/(double)TABLE_SIZE) * M_PI * 2.*441.);
    }
    _data.left_phase = _data.right_phase = 0;
   

    err = Pa_OpenStream(
              &stream,
              NULL, /* no input */
              &outputParameters,
              SAMPLE_RATE,
              FRAMES_PER_BUFFER,
              paClipOff,      /* we won't output out of range samples so don't bother clipping them */
              patestCallback,
              &_data );
    if( err != paNoError ) goto error;

    sprintf( _data.message, "No Message" );
    err = Pa_SetStreamFinishedCallback( stream, &StreamFinished );
    if( err != paNoError ) goto error;

    return err;
error:
    Pa_Terminate();
    fprintf( stderr, "An error occured while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", err );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
    return err;
}


int SoundIO::Off() {

/*	err = Pa_StopStream( stream );
	if( err != paNoError ) goto error;
*/
	err = Pa_CloseStream( stream );
	if( err != paNoError ) goto error;

	Pa_Terminate();
	printf("  |-- Disabling : PortAudio streaming.\n");

	return err;
error:
    Pa_Terminate();
    fprintf( stderr, "An error occured while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", err );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
    return err;
}

int SoundIO::Play() {

    err = Pa_StartStream( stream );
    if( err != paNoError ) goto error;

    return err;
error:
    Pa_Terminate();
    fprintf( stderr, "An error occured while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", err );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
    return err;
}

int SoundIO::Pause() {

	err = Pa_StopStream( stream );
	if( err != paNoError ) goto error;

    return err;
error:
    Pa_Terminate();
    fprintf( stderr, "An error occured while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", err );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
    return err;
}


int SoundIO::Send(vector<double> data) {
	_data._size_ = data.size();
cout << _data._size_ << endl;
	_data.sine = new float[_data._size_];
	for (int i(0); i < _data._size_; i++) {

		_data.sine[i] = (float)data[i];
	}
	return 0;
}


int SoundIO::test() {

    int i;

    printf("PortAudio Test: output sine wave. SR = %d, BufSize = %d\n", SAMPLE_RATE, FRAMES_PER_BUFFER);

    /* initialise sinusoidal wavetable */

    for( i=0; i<TABLE_SIZE; i++ )
    {
        _data.sine[i] = (float) sin( ((double)i/(double)TABLE_SIZE) * M_PI * 4. );
    }
    _data.left_phase = _data.right_phase = 0;

return 0;
}
