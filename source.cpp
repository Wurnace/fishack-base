#include <SDL.h>
#include <iostream>

#include "Points.h"
#include "vector.h"

struct point : public SDL_Point
{
public :
	point() {}
	point(float x, float y) :x(x), y(y) {}
	point(float x, float y, float velx, float vely) :x(x), y(y), velx(velx), vely(vely) {}
	float x = 0;
	float y = 0;
	float forcex, forcey = 0;
	float velx, vely = 0;
	float targetx = 0, targety = 0;
};

struct shape
{
	int start;
	int size;
	// size is the amount of points it is
};

point averagepoint(point* points, int start, int size)
{
	point average = { 0, 0 };
	for (int i = start; i < start + size; i++)
	{
		average.x += points[i].x / size;
		average.y += points[i].y / size;
		average.velx += points[i].velx;
		average.vely += points[i].vely;
	};
	//std::cout << average.x / size;
	return average;
}

void movetotarget(shape* shapes, point* points, int numshapes)
{
	for (int shapi = 0; shapi < numshapes; shapi++)
	{
		for (int i = shapes[shapi].start; i < shapes[shapi].start + shapes[shapi].size; i++)
		{
			point average = averagepoint(points, shapes[shapi].start, shapes[shapi].size);
			float targetx = points[i].targetx + average.x;
			float targety = points[i].targety + average.y;
			float dist = sqrt(((targetx - points[i].x) * (targetx - points[i].x)) + ((targety - points[i].y) * (targety - points[i].y)));
			if (dist == 0)
			{
				dist = 1;
			}
			//points[i].forcex = (targetx - points[i].x) / dist;
			//points[i].forcey = (targety - points[i].y) / dist;
			std::cout << targetx - points[i].x;
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

void move(point* allpoints, int numpoints)
{	
	for (int i = 0; i < numpoints; i++)
	{
		allpoints[i].velx += allpoints[i].forcex;
		allpoints[i].vely += allpoints[i].forcey;
		allpoints[i].velx = float(allpoints[i].velx * 0.4);
		allpoints[i].vely = float(allpoints[i].vely * 0.4);
		allpoints[i].forcex = 0;
		allpoints[i].forcey = 0;
		allpoints[i].x += allpoints[i].velx;
		allpoints[i].y += allpoints[i].vely;
	}
}

int main(int argc, char ** argv)
{
	int numpoints = 4;
	int numshapes = 1;
	point allpoints[4] = { {40, 40}, {80, 40}, {80, 80}, {40, 80} };
	allpoints[0].targetx = -20;
	allpoints[0].targety = -20;
	allpoints[1].targetx = 20;
	allpoints[1].targety = -20;
	allpoints[2].targetx = 20;
	allpoints[2].targety = 20;
	allpoints[3].targetx = -20;
	allpoints[3].targety = 20;
	shape shapes[1] = {{0, 4}};

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
		SDL_PollEvent(&event);

		if (event.type == SDL_QUIT)
		{
			quit = true;
		}

		// clear window

		SDL_SetRenderDrawColor(renderer, 242, 242, 242, 255);
		SDL_RenderClear(renderer);

		// TODO rendering code goes here
		SDL_SetRenderDrawColor(renderer, 250, 0, 0, 255);
//#define drawLine(ren, a, b, off, w, h) SDL_RenderDrawLine(ren, a.x - off.x + w, a.y - off.y + h, b.x - off.x + w, b.y - off.y + h);
#define drawLine(ren, a, b, off, w, h) SDL_RenderDrawLine(ren, int(a.x) + w, int(a.y) + h, int(b.x) + w, int(b.y) + h);
		for (int shapi = 0; shapi < numshapes; shapi++)
		{
			point average = averagepoint(allpoints, shapes[shapi].start, shapes[shapi].size);
			for (int i = shapes[shapi].start + 1; i < shapes[shapi].size + shapes[shapi].start; i++)
			{
				drawLine(renderer, allpoints[i], allpoints[i - 1], 0, 320, 240);
			};
			drawLine(renderer, allpoints[shapes[shapi].start], allpoints[shapes[shapi].start + shapes[shapi].size - 1], average, 320, 240);
		}
		move(allpoints, numpoints);
		movetotarget(shapes, allpoints, numshapes);

		SDL_RenderPresent(renderer);
	}

	// cleanup SDL

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
