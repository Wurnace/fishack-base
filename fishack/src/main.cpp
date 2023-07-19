#include <SDL.h>
#include <iostream>
#include <vector>

#include "Points.h"
#include "Vector2.h"
#include "Shape.h"

void movetotarget(std::vector<shape>& shapes, std::vector<point>& points)
{
	for (int shapi = 0; shapi < shapes.size(); shapi++)
	{
		shapes[shapi].movetotarget(points);
	}
}

void move(std::vector<point>& allpoints, float delta)
{
	for (int i = 0; i < allpoints.size(); i++)
	{
		allpoints[i].Update(delta);
	}
}

int main(int argc, char ** argv)
{
	Uint64 NOW = 0;
	Uint64 LAST = 0;
	double deltaTime = 0;
	std::vector<point> allpoints = { {-20, -20}, {20, -20}, {20, 20}, {-20, 20}, {-40, -40}, {40, -40}, {40, 40}, {-40, 40} };

	std::vector<shape> shapes = { {0, 4}, {4, 4} };
	for (auto & shapei : shapes)
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

	float average = 0;

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

		// Calculate DeltaTimes
		NOW = SDL_GetTicks();
		deltaTime = (NOW - LAST) * (15.0 / 1000.0f);
		if (deltaTime > average) deltaTime = average;
		if (deltaTime > 1.5) deltaTime = 1.5;
		average = average * 0.99f + (NOW + LAST)/2 * 0.01f;
		LAST = NOW;

		// Clear Window
		SDL_SetRenderDrawColor(renderer, 242, 242, 242, 255);
		SDL_RenderClear(renderer);

	#define drawLine(ren, a, b, off, w, h) SDL_RenderDrawLine(ren, int(a.x - off.x + w), int(a.y - off.y + h), int(b.x - off.x + w), int(b.y - off.y + h));
		for (int shapi = 0; shapi < shapes.size(); shapi++)
		{

			SDL_SetRenderDrawColor(renderer, 250, 0, 0, 255);
			point average = shapes[camerashape].averagepoint(allpoints);
			shapes[shapi].foreachPoint(allpoints, [&] (point& curPoint, point nextPoint) {
				drawLine(renderer, curPoint, nextPoint, average, 320, 240);
			});
		}

		movetotarget(shapes, allpoints);
		move(allpoints, float(deltaTime));

		for (shape shapei : shapes) {
			shapei.jiggle(allpoints);
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
