#pragma once
#include <stdlib.h>
struct AudioPosition {
	int interpolation;
	float x;
	float y;

	AudioPosition() : interpolation(1), x(0), y(0) {  }
	AudioPosition(const AudioPosition &other) : interpolation(other.interpolation), x(other.x), y(other.y) {}
};

struct AudioObject {
	int numPositions;
	AudioPosition *positions;
	float xScale;
	float yScale;
	float xTranslation;
	float yTranslation;

	AudioObject() : numPositions(0), positions(NULL), xScale(1), yScale(1), xTranslation(0), yTranslation(0) {}
	AudioObject(const AudioObject &other) : numPositions(other.numPositions), xScale(other.xScale), yScale(other.yScale), xTranslation(other.xTranslation), yTranslation(other.yTranslation) {
		positions = new AudioPosition[numPositions];
		for (int i = 0; i < numPositions; i++) {
			positions[i] = other.positions[i];
		}
	}

	~AudioObject() {
		if (numPositions != 0) {
			delete[] positions;
		}
	}
};