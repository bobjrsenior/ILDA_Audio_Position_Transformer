#pragma once
#include "Structs.h"
#include "AudioObjects.h"

class Enemy {
private:
	AudioDrawingFramework *audioFramework;
	int index;
	float radius;
	float xSpeed;
	float ySpeed;

private:
	void setRadius() {
		AudioObject *obj = audioFramework->getAudioObject(index);
		if (obj->numPositions > 0) {
			radius = obj->positions[0].x * obj->xScale;
		}
	}
	void setRadius(AudioObject *obj) {
		if (obj->numPositions > 0) {
			radius = obj->positions[0].x * obj->xScale;
		}
	}

public:

	Enemy(AudioDrawingFramework *audioFramework, int index) : audioFramework(audioFramework), index(index) {
		setRadius();
	}

	void setXSpeed(float speed) {
		xSpeed = speed;
	}

	void setYSpeed(float speed) {
		ySpeed = speed;
	}

	void setSpeed(float _xSpeed, float _ySpeed) {
		xSpeed = _xSpeed;
		ySpeed = _ySpeed;
	}

	void setXScale(float xScale) {
		AudioObject *obj = audioFramework->getAudioObject(index);
		obj->xScale = xScale;
		setRadius(obj);
	}

	void setYScale(float yScale) {
		AudioObject *obj = audioFramework->getAudioObject(index);
		obj->yScale = yScale;
		setRadius(obj);
	}

	void setScale(float xScale, float yScale) {
		AudioObject *obj = audioFramework->getAudioObject(index);
		obj->xScale = xScale;
		obj->yScale = yScale;
		setRadius(obj);
	}

	void setScale(float scale) {
		AudioObject *obj = audioFramework->getAudioObject(index);
		obj->xScale = scale;
		obj->yScale = scale;
		setRadius(obj);
	}

	void setXTranslation(float translation) {
		AudioObject *obj = audioFramework->getAudioObject(index);
		obj->xTranslation = translation;
	}

	void setYTranslation(float translation) {
		AudioObject *obj = audioFramework->getAudioObject(index);
		obj->yTranslation = translation;
	}

	void setTranslation(float _xTranslation, float _yTranslation) {
		AudioObject *obj = audioFramework->getAudioObject(index);
		obj->xTranslation = _xTranslation;
		obj->yTranslation = _yTranslation;
	}

	float getRadius() {
		return radius;
	}

	void move() {
		AudioObject *obj = audioFramework->getAudioObject(index);
		obj->xTranslation += xSpeed;
		obj->yTranslation += ySpeed;
	}

};