#pragma once
#include "Structs.h"
#include <stdlib.h>
#include <math.h>

#define PI 3.1415923f


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
	AudioObject digits[10];

public:
	AudioDrawingFramework() : maxAudioObjects(10), numAudioObjects(0), curAudioObject(0), objectSampleIndex(0), positionSampleCount(100), transitionCounter(-1) {
		audioObjects = new AudioObject[maxAudioObjects];
		generateDigits();
	}

	AudioDrawingFramework(int maxAudioObjects) : maxAudioObjects(maxAudioObjects), numAudioObjects(0), curAudioObject(0), objectSampleIndex(0), positionSampleCount(100), transitionCounter(-1) {
		audioObjects = new AudioObject[maxAudioObjects];
		generateDigits();
	}

	~AudioDrawingFramework() {
		// Deallocate the digits
		for (int i = 0; i < 10; i++) {
			if (digits[i].numPositions != 0) {
				delete[] digits[i].positions;
			}
		}

		// Deallocate every AudioObject's Positions
		for (int i = 0; i < numAudioObjects; i++) {
			if (audioObjects[i].numPositions != 0) {
				delete[] audioObjects[i].positions;
			}
		}
		// Deallocate the AudioObjects
		delete[] audioObjects;


	}

	int addAudioObject(AudioObject obj) {
		// If we need to grow
		if (numAudioObjects == maxAudioObjects) {
			// Allocate a larger buffer
			int newMaxSize = (int)(maxAudioObjects * 1.5f + 0.5f);
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

		int index = numAudioObjects;

		// Add in new object
		audioObjects[index] = obj;
		// Increment size
		numAudioObjects++;

		return index++;
	}

	void removeAudioObject(int index) {
		// Bounds check
		if ((unsigned) index < (unsigned) numAudioObjects){
			audioObjects[index].numPositions = 0;
			delete[] audioObjects[index].positions;
		}
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

				AudioPosition nextAudioPos = audioPos;
				if (objectSampleIndex + 1 < curObj.numPositions) {
					nextAudioPos = curObj.positions[objectSampleIndex + 1];
				}

				// If not currently transitioning 
				if (transitionCounter == -1) {
					transitionCounter = positionSampleCount;
				}

				// Interpolate positions
				if (audioPos.interpolation) {
					audioPos.x = (audioPos.x * ((float)transitionCounter / positionSampleCount)) + (nextAudioPos.x * (1.0f - ((float)transitionCounter / positionSampleCount)));
					audioPos.y = (audioPos.y * ((float)transitionCounter / positionSampleCount)) + (nextAudioPos.y * (1.0f - ((float)transitionCounter / positionSampleCount)));
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

				// Apply scaling
				audioPos.x *= curObj.xScale;
				audioPos.y *= curObj.yScale;

				// Apply translations
				audioPos.x += curObj.xTranslation;
				audioPos.y += curObj.yTranslation;

				// Clamping to [-1,1]
				if (audioPos.x > 1.0f) {
					audioPos.x = 1.0f;
				}
				else if (audioPos.x < -1.0f) {
					audioPos.x = -1.0f;
				}

				if (audioPos.y > 1.0f) {
					audioPos.y = 1.0f;
				}
				else if (audioPos.y < -1.0f) {
					audioPos.y = -1.0f;
				}

				return audioPos;
			}
		}
		return zeroPosition;
	}

	void setTransition(int transition) {
		positionSampleCount = transition;
	}

	AudioObject * getAudioObject(int index) {
		// Bounds test
		if ((unsigned) index < (unsigned)numAudioObjects) {
			return &audioObjects[index];
		}
		return NULL;
	}

	AudioObject * operator[](int index) {
		// Bounds test
		if (((unsigned)index) < (unsigned)numAudioObjects) {
			return &audioObjects[index];
		}
		return NULL;
	}

	AudioObject getDigit(int digit) {
		AudioObject temp;
		// Bounds check
		if ((unsigned) digit < (unsigned) 10) {
			temp = digits[digit];
		}
		return temp;
	}

	AudioObject makeCircle(int points) {
		return makeCircle(points, 1.0f);
	}

	AudioObject makeCircle(int points, float radius) {
		AudioObject circle;
		circle.xScale = radius;
		circle.yScale = radius;
		circle.numPositions = points + 1;
		circle.positions = new AudioPosition[points];

		// The number of radians between each point
		float deltaRadians = 2 * PI / points;

		// Current "position" on the circle
		float radians = 0;

		// Set circle positions
		for (int i = 0; i < points; i++, radians += deltaRadians) {
			circle.positions[i].x = cosf(radians);
			circle.positions[i].y = sinf(radians);
		}
		// Set last point to the first to avoid errors
		circle.positions[points].x = circle.positions[0].x;
		circle.positions[points].y = circle.positions[0].y;

		return circle;
	}

	private:

	void generateDigits() {
		int numPositions;
		AudioPosition *positions;

		// Digit 0
		numPositions = 5;
		digits[0].numPositions = numPositions;
		digits[0].positions = new AudioPosition[numPositions];
		positions = digits[0].positions;
		positions[0].x = -0.1f;
		positions[0].y = 0.2f;
		positions[1].x = 0.1f;
		positions[1].y = 0.2f;
		positions[2].x = 0.0f;
		positions[2].y = 0.0f;
		positions[3].x = -0.1f;
		positions[3].y = 0.0f;
		positions[4].x = -0.1f;
		positions[4].y = 0.2f;


		// Digit 1
		numPositions = 2;
		digits[1].numPositions = numPositions;
		digits[1].positions = new AudioPosition[numPositions];
		positions = digits[1].positions;
		positions[0].x = 0.0f;
		positions[0].y = 0.2f;
		positions[1].x = 0.0f;
		positions[1].y = 0.0f;

		// Digit 2
		numPositions = 6;
		digits[2].numPositions = numPositions;
		digits[2].positions = new AudioPosition[numPositions];
		positions = digits[2].positions;
		positions[0].x = -0.1f;
		positions[0].y = 0.2f;
		positions[1].x = 0.0f;
		positions[1].y = 0.2f;
		positions[2].x = 0.0f;
		positions[2].y = 0.1f;
		positions[3].x = -0.1f;
		positions[3].y = 0.1f;
		positions[4].x = -0.1f;
		positions[4].y = 0.0f;
		positions[5].x = 0.0f;
		positions[5].y = 0.0f;

		// Digit 3
		numPositions = 7;
		digits[3].numPositions = numPositions;
		digits[3].positions = new AudioPosition[numPositions];
		positions = digits[3].positions;
		positions[0].x = -0.1f;
		positions[0].y = 0.2f;
		positions[1].x = 0.0f;
		positions[1].y = 0.2f;
		positions[2].x = 0.0f;
		positions[2].y = 0.1f;
		positions[3].x = -0.1f;
		positions[3].y = 0.1f;
		positions[4].x = 0.0f;
		positions[4].y = 0.1f;
		positions[5].x = 0.0f;
		positions[5].y = 0.0f;
		positions[6].x = -0.1f;
		positions[6].y = 0.0f;

		// Digit 4
		numPositions = 5;
		digits[4].numPositions = numPositions;
		digits[4].positions = new AudioPosition[numPositions];
		positions = digits[4].positions;
		positions[0].x = -0.1f;
		positions[0].y = 0.2f;
		positions[1].x = -0.1f;
		positions[1].y = 0.1f;
		positions[2].x = 0.0f;
		positions[2].y = 0.1f;
		positions[2].interpolation = 0;
		positions[3].x = 0.0f;
		positions[3].y = 0.2f;
		positions[4].x = 0.0f;
		positions[4].y = 0.0f;

		// Digit 5
		numPositions = 6;
		digits[5].numPositions = numPositions;
		digits[5].positions = new AudioPosition[numPositions];
		positions = digits[5].positions;
		positions[0].x = -0.1f;
		positions[0].y = 0.2f;
		positions[1].x = 0.0f;
		positions[1].y = 0.2f;
		positions[2].x = 0.0f;
		positions[2].y = 0.1f;
		positions[3].x = -0.1f;
		positions[3].y = 0.1f;
		positions[4].x = -0.1f;
		positions[4].y = 0.0f;
		positions[5].x = 0.0f;
		positions[5].y = 0.0f;

		// Digit 6
		numPositions = 6;
		digits[6].numPositions = numPositions;
		digits[6].positions = new AudioPosition[numPositions];
		positions = digits[6].positions;
		positions[0].x = 0.0f;
		positions[0].y = 0.2f;
		positions[1].x = -0.1f;
		positions[1].y = 0.2f;
		positions[2].x = -0.1f;
		positions[2].y = 0.0f;
		positions[3].x = 0.0f;
		positions[3].y = 0.0f;
		positions[4].x = 0.0f;
		positions[4].y = 0.1f;
		positions[5].x = -0.1f;
		positions[5].y = 0.1f;

		// Digit 7
		numPositions = 3;
		digits[7].numPositions = numPositions;
		digits[7].positions = new AudioPosition[numPositions];
		positions = digits[7].positions;
		positions[0].x = 0.0f;
		positions[0].y = 0.2f;
		positions[1].x = 0.0f;
		positions[1].y = 0.2f;
		positions[2].x = 0.0f;
		positions[2].y = 0.0f;

		// Digit 8
		numPositions = 7;
		digits[8].numPositions = numPositions;
		digits[8].positions = new AudioPosition[numPositions];
		positions = digits[8].positions;
		positions[0].x = -0.1f;
		positions[0].y = 0.2f;
		positions[1].x = 0.0f;
		positions[1].y = 0.2f;
		positions[2].x = 0.0f;
		positions[2].y = 0.0f;
		positions[3].x = -0.1f;
		positions[3].y = 0.0f;
		positions[4].x = -0.1f;
		positions[4].y = 0.2f;
		positions[4].interpolation = 0;
		positions[5].x = -0.1f;
		positions[5].y = 0.1f;
		positions[6].x = 0.0f;
		positions[6].y = 0.1f;

		// Digit 9
		numPositions = 5;
		digits[9].numPositions = numPositions;
		digits[9].positions = new AudioPosition[numPositions];
		positions = digits[9].positions;
		positions[0].x = 0.0f;
		positions[0].y = 0.1f;
		positions[1].x = -0.1f;
		positions[1].y = 0.1f;
		positions[2].x = -0.1f;
		positions[2].y = 0.2f;
		positions[3].x = 0.0f;
		positions[3].y = 0.2f;
		positions[4].x = 0.0f;
		positions[4].y = 0.0f;
	}
};