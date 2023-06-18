#include <SDL.h>
#include <iostream>

#include "Points.h"
#include "vector.h"

point averagepoint(point* points, int start, int size)
{
	point average = { 0, 0 };
	for (int i = start; i < start + size; i++)
	{
		average += points[i].getVector();
	};
	average /= size;
	return average;
}

float averageangle(point* points, int start, int size)
{
	float average = 0;
	point averagep = averagepoint(points, start, size);
	for (int i = start; i < start + size; i++)
	{
		average -= averagep.angleBetween(points[i]);
		average += averagep.angleBetween(averagep + points[i].targetOff);
	};
	return average / size;
}

struct shape
{
	int start;
	int size;
	// size is the amount of points it is

	void assignOffset(point* points)
	{
		point average = averagepoint(points, this->start, this->size);
		for (int i = this->start; i < this->start + this->size; i++)
		{
			points[i].setOffset(points[i] - average);
		};
	}

	void jiggle(point* points)
	{
		for (int i = this->start; i < this->start + this->size; i++)
		{
			Vector random = { float(rand() % 66 - 5), float(rand() % 66 - 5) };
			points[i] += random;
		};
	}
	void movetotarget(point* points)
	{
		point average = averagepoint(points, this->start, this->size);
		float angleaverage = averageangle(points, this->start, this->size);
		for (int i = this->start; i < this->start + this->size; i++)
		{
			Vector OffsetCopy = (points[i].targetOff).rotateNew(angleaverage, {0, 0});
			//if (OffsetCopy.mag() - (points[i].targetOff).mag() <= 0.001) continue;
			points[i].moveToTarget(average + OffsetCopy);
		}
	}
};

void movetotarget(shape* shapes, point* points, int numshapes)
{
	for (int shapi = 0; shapi < numshapes; shapi++)
	{
		for (int i = shapes[shapi].start; i < shapes[shapi].start + shapes[shapi].size; i++)
		{
			shapes[shapi].movetotarget(points);
		}
	}
}

/*float getarea(point* points)
{
	int xpoints = sizeof(points);
	float area = 0.0;
	int j = xpoints - 1;
	for (int i = 0; i < xpoints; i++)
	{
		area += (points[j].x + points[i].x) * (points[j].y - points[i].y);
		j = i;
	};
	return abs(area) / 2.0;
}*/

void move(point* allpoints, int numpoints, float deltaTime)
{
	for (int i = 0; i < numpoints; i++)
	{
		allpoints[i].Update(deltaTime);
	}
}

int main(int argc, char ** argv)
{
	Uint64 NOW = SDL_GetPerformanceCounter();
	Uint64 LAST = 0;
	double deltaTime = 0;
	int numpoints = 4;
	int numshapes = 1;
	point allpoints[4] = { {40, 40}, {80, 40}, {80, 80}, {40, 80} };
	shape shapes[1] = { {0, 4}};

	for (shape shapei : shapes)
	{
		shapei.assignOffset(allpoints);
		shapei.jiggle(allpoints);
	}

	int camerashape = 0;

	// variables
	bool quit = false;
	SDL_Event event;

	// init SDL

	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window * window = SDL_CreateWindow("j",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
	SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);

	while (!quit)
	{
		SDL_Delay(10);
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}
			// input here
			if (event.type == SDL_KEYDOWN)
			{
				// key inputs
				if (event.key.keysym.sym == SDLK_1)
				{
					std::cout << "keypressed" << std::endl;
					if (camerashape == numshapes - 1)
					{
						camerashape = 0;
					}
					else {
						camerashape++;
					}
				}
			}
		}

		// clear window
		LAST = NOW;
		NOW = SDL_GetPerformanceCounter();

		deltaTime = (double)((NOW - LAST) * 30 / (double)SDL_GetPerformanceFrequency());

		SDL_SetRenderDrawColor(renderer, 242, 242, 242, 255);
		SDL_RenderClear(renderer);

		// TODO rendering code goes here
#define drawLine(ren, a, b, off, w, h) SDL_RenderDrawLine(ren, int(a.x - off.x + w), int(a.y - off.y + h), int(b.x - off.x + w), int(b.y - off.y + h));
//#define drawLine(ren, a, b, off, w, h) SDL_RenderDrawLine(ren, int(a.x) + w, int(a.y) + h, int(b.x) + w, int(b.y) + h);
		for (int shapi = 0; shapi < numshapes; shapi++)
		{
			SDL_SetRenderDrawColor(renderer, 250, 0, 0, 255);
			point average = averagepoint(allpoints, shapes[camerashape].start, shapes[camerashape].size);
			for (int i = shapes[shapi].start + 1; i < shapes[shapi].size + shapes[shapi].start; i++)
			{
				drawLine(renderer, allpoints[i], allpoints[i - 1], average, 320, 240);
			};
			SDL_SetRenderDrawColor(renderer, 0, 0, 250, 255);
			drawLine(renderer, allpoints[shapes[shapi].start], allpoints[shapes[shapi].start + shapes[shapi].size - 1], average, 320, 240);
		}
		move(allpoints, numpoints, float(deltaTime));
		movetotarget(shapes, allpoints, numshapes);

		SDL_RenderPresent(renderer);
	}

	// cleanup SDL

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
