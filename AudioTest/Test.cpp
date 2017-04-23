#include <portaudio.h>
#include "AudioObjects.h"
#include <iostream>
#include <vector>
#include <time.h>
#ifdef _WIN32
#include <curses.h>
#else
#include <ncurses.h>
#endif
#include "Enemy.h"


#define SAMPLE_RATE 30000

static AudioDrawingFramework audioFrameWork;

static float xPos = 0;
static float yPos = 0;
float speed = 0.1f;
// Out of 100
int spawnChance = 20;

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
		AudioPosition audioPos = data->getNextSample();

		*out++ = audioPos.y; // Left
		*out++ = -audioPos.x; // Right

	}
	return 0;
}

int main(void) {
	//Seed random
	srand((unsigned int) time(NULL));
	// Initialization
	int error = Pa_Initialize();
	if (error != paNoError) {
		return error;
	}

	// Lists of objects
	std::vector<Enemy> enemies;

	//Setup NCurses
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	WINDOW * win = newwin(30, 30, 0, 0);

	mousemask(REPORT_MOUSE_POSITION, NULL);

	int mouseX;
	int mouseY;

	AudioObject enemyTemplate = audioFrameWork.makeCircle(5, 0.2f);

	// Create the AudioObject
	AudioObject objTest;

	// Give it four positions
	objTest.numPositions = 5;
	objTest.positions = new AudioPosition[objTest.numPositions];
	objTest.xTranslation = 0.0f;
	objTest.yTranslation = 0.0f;
	// Fill in the positions

	objTest.positions[0].interpolation = 0;
	objTest.positions[0].x = -1.0f;
	objTest.positions[0].y = -1.0f;


	objTest.positions[1].interpolation = 0;
	objTest.positions[1].x = -1.0f;
	objTest.positions[1].y = 1.0f;

	objTest.positions[2].interpolation = 0;
	objTest.positions[2].x = 1.0f;
	objTest.positions[2].y = 1.0f;

	objTest.positions[3].interpolation = 0;
	objTest.positions[3].x = 1.0f;
	objTest.positions[3].y = -1.0f;

	objTest.positions[4].interpolation = 0;
	objTest.positions[4].x = -1.0f;
	objTest.positions[4].y = -1.0f;

	int reticleIndex = audioFrameWork.addAudioObject(objTest);

	Enemy reticle(&audioFrameWork, reticleIndex);
	//reticle.setSpeed(0.1f, 0.0f);
	// reticle.setXTranslate()


		// Open audio stream
		PaStream *audioStream;
		error = Pa_OpenDefaultStream(&audioStream, 0, 2, paFloat32, SAMPLE_RATE, 1, patestCallback, &audioFrameWork);

		// Output to audio stream
		error = Pa_StartStream(audioStream);
		if (error != paNoError) {
			return error;
		}

	while( true )
    {
        getyx(win,mouseY,mouseX);
				mvprintw(0,0,"Mouse\t\tx:%d\ty:%d", mouseX, mouseY);
				mvprintw(2,0,"Reticle\t\tx:%d\ty:%d", reticle.getXPosition(), reticle.getYPosition());
				char ch = getch();
				mvprintw(3,0,"Char: %d", ch);
				refresh();

        switch(ch)
        {
            case 4:
								reticle.translateX(-speed);
								mvprintw(4,0,"KeyLeft"); refresh();
                break;
            case 5:
								reticle.translateX(speed);
								mvprintw(4,0,"KeyRight"); refresh();
                break;
            case 3:
								reticle.translateY(speed);
								mvprintw(4,0,"KeyUp"); refresh();
                break;
            case 2:
								reticle.translateY(-speed);
								mvprintw(4,0,"KeyDown"); refresh();
                break;
            case KEY_F(1):
                exit(1);
        }

		// Check if a new enemy should spawn
		if (rand() % 100 < spawnChance) {
			AudioObject newEnemyObj(enemyTemplate);
			int index = audioFrameWork.addAudioObject(newEnemyObj);
			Enemy newEnemy(&audioFrameWork, index);
			enemies.push_back(newEnemy);
			newEnemy.setTranslation(2.0f, 2.0f);
			newEnemy.setYSpeed(-0.01f);
		}

		// Cycle through each enemy
		for (int i = 0; i < (int) enemies.size(); i++) {
			Enemy enemy = enemies[i];
			if (enemy.getYPosition() < -1.5f) {
				enemy.move();
				enemies.erase(enemies.begin() + i);
				i--;
				audioFrameWork.removeAudioObject(enemy.getIndex());
			}
		}
    }



	// Sleep for a bit (5 seconds)
	//Pa_Sleep(5 * 1000);


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


	endwin();
	return 0;
}
