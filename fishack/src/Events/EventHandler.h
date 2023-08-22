#pragma once

#include <SDL.h>
#include <functional> // Use Callback functions
#include <map>

FishackBegin

class EventHandler
{
public:
	EventHandler();

	void Run(SDL_Event& evt) const;

	void Push(Uint32 type, std::function<void(SDL_Event&)> func);
	void Push(SDL_Event& evt, std::function<void(SDL_Event&)> func);

	std::map<Uint32, std::vector< std::function<void(SDL_Event&)> >> Events;
};

FishackEnd
