#include <SDL.h>
#include <iostream>

#include "Points.h"
#include "vector.h"
#include "Shape.h"

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

void move(point* allpoints, int numpoints, float delta)
{
	for (int i = 0; i < numpoints; i++)
	{
		allpoints[i].Update(delta);
	}
}

int main(int argc, char ** argv)
{
	Uint64 NOW = 0;
	Uint64 LAST = 0;
	double deltaTime = 0;
	int numpoints = 8;
	int numshapes = 2;
	point allpoints[10] = { {-20, -20}, {20, -20}, {20, 20}, {-20, 20}, {-40, -40}, {40, -40}, {40, 40}, {-40, 40} };

	shape shapes[2] = { {0, 4}, {4, 4} };
	for (shape shapei : shapes)
	{
		shapei.assignOffset(allpoints);
		shapei.jiggle(allpoints);
	}

	int camerashape = 0;

	// [Window] Variables
	bool quit = false;
	SDL_Event event;

	// Init SDL
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window * window = SDL_CreateWindow("Fishack",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
	SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);

	while (!quit)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}
			// input here
			if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_SPACE) {
					goto closeWindow;
				}
			}
		}

		// Calculate DeltaTime
		NOW = SDL_GetTicks();
		deltaTime = (NOW - LAST) * (15.0 / 1000.0f);
		if (deltaTime > 1.5) deltaTime = 1.5;
		LAST = NOW;

		// Clear Window
		SDL_SetRenderDrawColor(renderer, 242, 242, 242, 255);
		SDL_RenderClear(renderer);

	#define drawLine(ren, a, b, off, w, h) SDL_RenderDrawLine(ren, int(a.x - off.x + w), int(a.y - off.y + h), int(b.x - off.x + w), int(b.y - off.y + h));
	// #define drawLine(ren, a, b, w, h) SDL_RenderDrawLine(ren, int(a.x) + w, int(a.y) + h, int(b.x) + w, int(b.y) + h);
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
		movetotarget(shapes, allpoints, numshapes);
		move(allpoints, numpoints, float(deltaTime));
		for (shape shapei : shapes) {
			//shapei.jiggle(allpoints);
		}

		SDL_RenderPresent(renderer);
	}

	// cleanup SDL

closeWindow:
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	if (!quit) return main(argc, argv);

	return 0;
}
