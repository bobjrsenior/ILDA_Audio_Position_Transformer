#pragma once
#include <stdlib.h>
typedef struct AudioPositionStruct {
	int interpolation;
	float x;
	float y;

	AudioPositionStruct() : interpolation(1), x(0), y(0) {  }
}AudioPosition;

typedef struct AudioObjectStruct {
	int numPositions;
	AudioPosition *positions;
	float xScale;
	float yScale;
	float xTranslation;
	float yTranslation;

	AudioObjectStruct() : numPositions(0), positions(NULL), xScale(1), yScale(1), xTranslation(0), yTranslation(0) {}
}AudioObject;