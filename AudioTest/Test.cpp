#include <portaudio.h>
#include "AudioObjects.h"
#include <iostream>

#define SAMPLE_RATE 30000


typedef struct
{
	float left_phase;
	float right_phase;
}
paTestData;

float values[2] = { 0.0f, 1.0f };

static paTestData data;

static int curValueIndex = 0;
static int samplesPerValue = 1;

static AudioDrawingFramework audioFrameWork;


/* This routine will be called by the PortAudio engine when audio is needed.
It may called at interrupt level on some machines so don't do anything
that could mess up the system like calling malloc() or free().
*/
static int patestCallback(const void *inputBuffer, void *outputBuffer,
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void *userData)
{

	/* Cast data passed through stream to our structure. */
	AudioDrawingFramework *data = (AudioDrawingFramework*)userData;
	float *out = (float*)outputBuffer;
	unsigned int i;
	(void)inputBuffer; /* Prevent unused variable warning. */

	for (i = 0; i<framesPerBuffer; i++)
	{
		//*out++ = data->left_phase;  /* left */
		//*out++ = data->right_phase;  /* right */


		/*
		if (curValueIndex++ == samplesPerValue) {
			curValueIndex = 0;

			data->left_phase = -data->left_phase;
			data->right_phase = -data->right_phase;
		}*/

		//*out++ = values[curValueIndex % 2];  /* left */
		//*out++ = values[curValueIndex % 2];  /* right */

		//curValueIndex++;

		AudioPosition audioPos = data->getNextSample();

		*out++ = audioPos.x; // Left
		*out++ = audioPos.y; // Right

	}
	return 0;
}

int main(void) {
	// Initialization
	int error = Pa_Initialize();
	if (error != paNoError) {
		return error;
	}

	data.left_phase = 0.5f;
	data.right_phase = 0.5f;

	// Create the AudioObject
	AudioObject objTest;
	/*
	// Give it four positions
	objTest.numPositions = 5;
	objTest.positions = new AudioPosition[objTest.numPositions];
	// Fill in the positions

	objTest.positions[0].interpolation = 20;
	objTest.positions[0].x = -1.0f;
	objTest.positions[0].y = -1.0f;

	objTest.positions[1].interpolation = 20;
	objTest.positions[1].x = -1.0f;
	objTest.positions[1].y = 1.0f;

	objTest.positions[2].interpolation = 20;
	objTest.positions[2].x = 1.0f;
	objTest.positions[2].y = 1.0f;

	objTest.positions[3].interpolation = 20;
	objTest.positions[3].x = 1.0f;
	objTest.positions[3].y = -1.0f;

	objTest.positions[4].interpolation = 20;
	objTest.positions[4].x = -1.0f;
	objTest.positions[4].y = -1.0f;

	audioFrameWork.addAudioObject(objTest);
	*/

	// Next Audio Object

	// Give it four positions
	objTest.numPositions = 4;
	objTest.positions = new AudioPosition[objTest.numPositions];
	// Fill in the positions

	objTest.positions[0].interpolation = 20;
	objTest.positions[0].x = -1.0f;
	objTest.positions[0].y = -1.0f;

	objTest.positions[1].interpolation = 20;
	objTest.positions[1].x = 0;
	objTest.positions[1].y = 1.0f;

	objTest.positions[2].interpolation = 20;
	objTest.positions[2].x = 1.0f;
	objTest.positions[2].y = -1.0f;

	objTest.positions[3].interpolation = 20;
	objTest.positions[3].x = -1.0f;
	objTest.positions[3].y = -1.0f;

	audioFrameWork.addAudioObject(objTest);

	// Open audio stream
	PaStream *audioStream;
	error = Pa_OpenDefaultStream(&audioStream, 0, 2, paFloat32, SAMPLE_RATE, 1, patestCallback, &audioFrameWork);
	
	// Output to audio stream
	error = Pa_StartStream(audioStream);
	if (error != paNoError) {
		return error;
	}


	// Sleep for a bit (20 seconds)
	Pa_Sleep(5 * 1000);

	// Stop stream
	error = Pa_StopStream(audioStream);
	if (error != paNoError) {
		return error;
	}

	// Close Stream
	error = Pa_CloseStream(audioStream);
	if (error != paNoError) {
		return error;
	}

	// Cleanup
	error = Pa_Terminate();
	if (error != paNoError) {
		return error;
	}
	return 0;
}