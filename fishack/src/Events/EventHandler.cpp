#ifndef FishackBegin
	#define FishackBegin namespace Fishack {
	#define FishackEnd }
#endif

#include "EventHandler.h"

FishackBegin

EventHandler::EventHandler()
{
}

/*
 * Run the Associated Function, depending on the event type. 
 *
 * @param event - SDL_Event
 */
void EventHandler::Run(SDL_Event& evt) const
{
	if (this->Events.find(evt.type) == this->Events.end()) return;
	for (auto& func : Events.at(evt.type))
	{
		func(evt);
	}
}

/*
 * Push the Function to the Event Stack, depending on the event type.
 *
 * @param type - SDL_Event type
 * @param func - Function
 */
void EventHandler::Push(Uint32 type, std::function<void(SDL_Event&)> func)
{
	if (this->Events.find(type) == this->Events.end())
	{
		this->Events[type] = { func };
		return;
	}
	this->Events.at(type).push_back(func);
}

/*
 * Push the Function to the Event Stack, depending on the event type.
 *
 * @param event - SDL_Event event
 * @param func - Function
 */
void EventHandler::Push(SDL_Event& evt, std::function<void(SDL_Event&)> func)
{
	this->Push(evt.type, func);
}

FishackEnd
