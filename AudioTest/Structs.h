#pragma once
#include <stdlib.h>
struct AudioPosition {
	int interpolation;
	float x;
	float y;

	AudioPosition() : interpolation(1), x(0), y(0) {  }
};

struct AudioObject {
	int temp;
	volatile int numPositions;
	AudioPosition *positions;
	float xScale;
	float yScale;
	float xTranslation;
	float yTranslation;

	AudioObject() : temp(0), numPositions(0), positions(NULL), xScale(1), yScale(1), xTranslation(0), yTranslation(0) {}
	AudioObject(const AudioObject &other) : temp(0), numPositions(other.numPositions), xScale(other.xScale), yScale(other.yScale), xTranslation(other.xTranslation), yTranslation(other.yTranslation) {
		positions = new AudioPosition[numPositions];
		for (int i = 0; i < numPositions; i++) {
			positions[i] = other.positions[i];
		}
	}

	~AudioObject() {
		if (temp == 0 && numPositions != 0) {
			delete[] positions;
			numPositions = 0;
		}
		else {
			int x = 5;
		}
	}
};