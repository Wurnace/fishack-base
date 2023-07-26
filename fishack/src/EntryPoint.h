#pragma once

#include <SDL.h>

#include "Shape.h"
#include "Points.h"
#include "Vector2.h"
#include "ShapeGroup.h"

#include "RandomNum.h"

FishackBegin

// time(NULL) returns seconds passed since 00:00:00 GMT, Jan 1, 1970

int entryPoint(int argc, char ** argv)
{
	RandomSeed();
	Uint64 NOW = 0;
	Uint64 LAST = 0;
	float deltaTime = 0;

	std::vector<ShapeGroup> allShapeGroups;
	allShapeGroups = { {} };

	allShapeGroups[0].Addpoints({ { -20, -20 }, { 20, -20 }, { 20, 20 }, { -20, 20 } });
	allShapeGroups[0].Addpoints({ { -40, -40 }, { 40, -40 }, { 40, 40 }, { -40, 40 } });

	allShapeGroups[0].Addshapes({ {0, 4}, {4, 4} });

	for (auto & shapei : allShapeGroups[0].shapes)
	{
		shapei.addShapeToPoints(allShapeGroups[0].points);
		shapei.assignOffset(allShapeGroups[0].points);
		shapei.jiggle(allShapeGroups[0].points, 4.0f);
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

	float average = 1;

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
				if (event.key.keysym.sym == SDLK_0) {
					allShapeGroups[0].deleteShape(0);
				}
				if (event.key.keysym.sym == SDLK_1) {
					allShapeGroups[0].deleteShape(1);
				}
			}
		}

		// Calculate DeltaTimes
		NOW = SDL_GetTicks();
		deltaTime = float(NOW - LAST) * (15.0f / 1000.0f);
		if (deltaTime > average) deltaTime = average;
		if (deltaTime > 1.5) deltaTime = 1.5;
		average = average * 0.9f + deltaTime * 0.1f;
		LAST = NOW;

		// Clear Window
		SDL_SetRenderDrawColor(renderer, 242, 242, 242, 255);
		SDL_RenderClear(renderer);

		// Render
		allShapeGroups[0].show(*renderer);

		// Update
		allShapeGroups[0].Update(deltaTime);

		for (shape shp : allShapeGroups[0].shapes)
		{
			shp.jiggle(allShapeGroups[0].points, 0.25f);
		}

		SDL_RenderPresent(renderer);
	}


closeWindow:
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	if (!quit) return main(argc, argv);

	return 0;
}

FishackEnd
