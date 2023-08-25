#pragma once

#include <SDL.h>

#include "Shapes/AllShapes.h"
#include "Events/EventHandler.h"

#include "Math/RandomNum.h"

#include "Renderer.h"

FishackBegin

int entryPoint(int argc, char ** argv)
{
	RandomSeed();

	Uint64 NOW = 0;
	Uint64 LAST = 0;
	float deltaTime = 0;

	std::vector<ShapeGroup> allShapeGroups;
	allShapeGroups = { {} };

	allShapeGroups[0].call_on_make_Shape([&allShapeGroups](Shape& Shapei)
	{
		Shapei.addShapeToPoints(allShapeGroups[0].points);
		Shapei.assignOffset(allShapeGroups[0].points);
		// Shapei.jiggle(allShapeGroups[0].points, 0.0f);
	});

	allShapeGroups[0].AddShape(Square(280, 80, 80));
	allShapeGroups[0].AddShape(Square(45, 65, 80));

	allShapeGroups[0].Shapes[0].foreachPoint(allShapeGroups[0].points, [&](point& current){
		//current.vel += {-26, 0};
		});

	   //int cameraShape = 0;
	/*
		For Now, cameraShape is always 0. 
		This will be redefined when[/if] A Renderer Class is added. 
	*/

	// [Window] Variables
	bool quit = false;
	bool restartProject = false;
	SDL_Event Event;
	EventHandler m_EventHandler;

	// Init SDL
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window * window = SDL_CreateWindow("Fishack",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
	SDL_Renderer* m_renderer = SDL_CreateRenderer(window, -1, 0);
	
	Renderer renderer(m_renderer);

	// Add Events to Stack

	//NOTE: Use SDL_GetKeyName()
	m_EventHandler.Push(SDL_KEYDOWN, [&](SDL_Event& evt) {
		if (Event.key.keysym.sym == SDLK_SPACE) {
			quit = true;
			restartProject = true;
		}
	});
	m_EventHandler.Push(SDL_KEYDOWN, [&](SDL_Event& evt) {
		std::cout << "// - From " << allShapeGroups[0].Shapes.size() << std::endl;
		if (Event.key.keysym.sym == SDLK_0) {
			allShapeGroups[0].deleteShapeByID(allShapeGroups[0].getShapeID(0));
		}
		if (Event.key.keysym.sym == SDLK_1) {
			allShapeGroups[0].deleteShapeByID(allShapeGroups[0].getShapeID(1));
		}
		std::cout << "// - To " << allShapeGroups[0].Shapes.size() << std::endl;
	});

	m_EventHandler.Push(SDL_MOUSEBUTTONDOWN, [&](SDL_Event& evt) {
		std::cout << "// + From " << allShapeGroups[0].Shapes.size() << std::endl;
		if (Event.button.button == SDL_BUTTON_LEFT && Event.button.clicks == 1)
		{
			Vector pos((float)Event.button.x, (float)Event.button.y);
			int mean_id = allShapeGroups[0].getShapeID(0);
			point average = {};
			if (mean_id >= 0)
			{
				average = allShapeGroups[0].Shapes[mean_id].averagepoint(allShapeGroups[0].points);
			}

			Vector center = average.getVector() - Vector(320, 240);
			int idx = (int)allShapeGroups[0].Shapes.size();
			allShapeGroups[0].AddShape(Polygon((pos + center).x, (pos + center).y, 60, 100, 123));
			allShapeGroups[0].ShapeAt(idx).jiggle(allShapeGroups[0].points, 5);
		}
		std::cout << "// + To " << allShapeGroups[0].Shapes.size() << std::endl;
	});

	while (!quit)
	{
		while (SDL_PollEvent(&Event))
		{
			if (Event.type == SDL_QUIT)
			{
				quit = true;
				break;
			}
			m_EventHandler.Run(Event);
		}

		// Calculate DeltaTimes
		NOW = SDL_GetTicks();
		deltaTime = float(NOW - LAST) * (15.0f / 1000.0f);
		if (deltaTime > 1.5) deltaTime = 1.5;
		LAST = NOW;

		// Clear Window
		SDL_SetRenderDrawColor(m_renderer, 242, 242, 242, 255);
		SDL_RenderClear(m_renderer);

		// Render
		allShapeGroups[0].show(renderer);

		// Update

		allShapeGroups[0].resolveCollision(allShapeGroups[0].getShapeID(0), allShapeGroups[0].ShapeAt(1), allShapeGroups[0].points);
		allShapeGroups[0].Update(deltaTime);

		/*
		float force = 1.0f;
		for (auto&[id, shape] : allShapeGroups[0].Shapes)
		{
			shape.jiggle(allShapeGroups[0].points, force);
		}
		*/

		SDL_RenderPresent(m_renderer);
	}

	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	if (restartProject) return main(argc, argv);

	return 0;
}

FishackEnd
