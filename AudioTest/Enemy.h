#pragma once
#include "Structs.h"
#include "AudioObjects.h"

class Enemy {
private:
	AudioDrawingFramework audioFrameWork;
	int index;
	float radius;
	float xSpeed;
	float ySpeed;

private:
	void setRadius() {
		AudioObject *obj = audioFrameWork[index];
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

	Enemy(AudioDrawingFramework audioFramework, int index) : audioFrameWork(audioFrameWork), index(index) {
		setRadius();
	}

	void setYSpeed(float speed) {
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
		AudioObject *obj = audioFrameWork[index];
		obj->xScale = xScale;
		setRadius(obj);
	}

	void setYScale(float yScale) {
		AudioObject *obj = audioFrameWork[index];
		obj->yScale = yScale;
		setRadius(obj);
	}

	void setScale(float xScale, float yScale) {
		AudioObject *obj = audioFrameWork[index];
		obj->xScale = xScale;
		obj->yScale = yScale;
		setRadius(obj);
	}

	void setScale(float scale) {
		AudioObject *obj = audioFrameWork[index];
		obj->xScale = scale;
		obj->yScale = scale;
		setRadius(obj);
	}

	float getRadius() {
		return radius;
	}

	void move() {
		AudioObject *obj = audioFrameWork[index];
		obj->xTranslation += xSpeed;
		obj->yTranslation += ySpeed;
	}

};