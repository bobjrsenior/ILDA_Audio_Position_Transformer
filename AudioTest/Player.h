#pragma once
#include "Enemy.h"

class Player : Enemy {

private:
	int health;
public:

	Player(AudioDrawingFramework *audioFramework, int index, int health) : Enemy(audioFramework, index), health(health) {}

};