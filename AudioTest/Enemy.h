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
		radius = obj->xScale;
	}
	void setRadius(AudioObject *obj) {
		radius = obj->xScale;
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

	void translateX(float amount) {
		AudioObject *obj = audioFramework->getAudioObject(index);
		obj->xTranslation += amount;
	}

	void translateY(float amount) {
		AudioObject *obj = audioFramework->getAudioObject(index);
		obj->yTranslation += amount;
	}

	void translateXY(float xAmount, float yAmount) {
		AudioObject *obj = audioFramework->getAudioObject(index);
		obj->xTranslation += xAmount;
		obj->yTranslation += yAmount;
	}

	float getRadius() {
		return radius;
	}

	float getXPosition() {
		AudioObject *obj = audioFramework->getAudioObject(index);
		return obj->xTranslation;
	}

	float getYPosition() {
		AudioObject *obj = audioFramework->getAudioObject(index);
		return obj->yTranslation;
	}

	void move() {
		AudioObject *obj = audioFramework->getAudioObject(index);
		obj->xTranslation += xSpeed;
		obj->yTranslation += ySpeed;
	}

	int isColliding(Enemy other) {
		// Pythagorean Theroem
		float xDistance = other.getXPosition() - getXPosition();
		float yDistance = other.getYPosition() - getXPosition();
		float maxDistanceSquared = getRadius() + other.getRadius();
		maxDistanceSquared *= maxDistanceSquared;
		if (xDistance * xDistance + yDistance * yDistance <= maxDistanceSquared) {
			return 1;
		}
		return 0;
	}

};