#pragma once
#include <stdlib.h>

typedef struct {
	int interpolation;
	float x;
	float y;

}AudioPosition;

typedef struct {
	int numPositions;
	AudioPosition *positions;
}AudioObject;


class AudioDrawingFramework {
private:
	int maxAudioObjects;
	int numAudioObjects;
	AudioObject *audioObjects;
	AudioPosition zeroPosition;
	int curAudioObject;
	int objectSampleIndex;
	int positionSampleCount;
	int transitionCounter;

public:
	AudioDrawingFramework() : maxAudioObjects(10), numAudioObjects(0), curAudioObject(0), objectSampleIndex(0), positionSampleCount(100), transitionCounter(-1) {
		audioObjects = new AudioObject[maxAudioObjects];
		zeroPosition = { 0, 0.0f, 0.0f };
	}

	AudioDrawingFramework(int maxAudioObjects) : maxAudioObjects(maxAudioObjects), numAudioObjects(0), curAudioObject(0), objectSampleIndex(0), positionSampleCount(100), transitionCounter(-1) {
		audioObjects = new AudioObject[maxAudioObjects];
		zeroPosition = { 0, 0.0f, 0.0f };
	}

	~AudioDrawingFramework() {
		// Deallocate every AudioObject's Positions
		for (int i = 0; i < numAudioObjects; i++) {
			delete[] audioObjects[i].positions;
		}
		// Deallocate the AudioObjects
		delete[] audioObjects;
	}

	void addAudioObject(AudioObject obj) {
		// If we need to grow
		if (numAudioObjects == maxAudioObjects) {
			// Allocate a larger buffer
			int newMaxSize = (int) (maxAudioObjects * 1.5f + 0.5f);
			// Create links to new/old buffers
			AudioObject *temp = new AudioObject[newMaxSize];
			AudioObject *old = audioObjects;

			// Copy old buffer data into the new buffer
			for (int i = 0; i < maxAudioObjects; i++) {
				temp[i] = old[i];
			}

			// Point audio array to the new buffer
			audioObjects = temp;
			// Set the new maxSize
			maxAudioObjects = newMaxSize;
			// Deallocate the old buffer
			delete[] old;
		}

		// Add in new object
		audioObjects[numAudioObjects] = obj;
		// Increment size
		numAudioObjects++;
	}

	AudioPosition getNextSample() {
		// Will be false if numAudioObjects is 0
		if (curAudioObject < numAudioObjects) {
			// Get the AudioObject
			AudioObject curObj = audioObjects[curAudioObject];

			// Will be false if object has no positions 
			if (objectSampleIndex < curObj.numPositions) {

				// Get the AudioPosition
				AudioPosition audioPos = curObj.positions[objectSampleIndex];

				// If not currently transitioning 
				if (transitionCounter == -1) {
					transitionCounter = positionSampleCount;
				}

				transitionCounter--;

				// If done transitioning
				if (transitionCounter < 0) {
					// Reset transition counter
					transitionCounter = -1;

					// Go to the next sample
					objectSampleIndex++;

					// Make sure we are at a valid sample position
					if (objectSampleIndex == curObj.numPositions) {
						objectSampleIndex = 0;
						curAudioObject++;
						if (curAudioObject == numAudioObjects) {
							curAudioObject = 0;
						}
					}
				}
				return audioPos;
			}
		}
		return zeroPosition;
	}

	void setTransition(int transition) {
		positionSampleCount = transition;
	}
};