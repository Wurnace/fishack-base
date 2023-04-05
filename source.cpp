#include <SDL.h>
#include <iostream>

struct point : public SDL_Point
{
public :
	point() {}
	point(double x, double y) :x(x), y(y) {}
	point(double x, double y, double velx, double vely) :x(x), y(y), velx(velx), vely(vely) {}
	double x = 0;
	double y = 0;
	double forcex, forcey = 0;
	double velx, vely = 0;
};

struct shape
{
	int start;
	int size;
	// size is the amount of points it is
};

point averagepoint(point* points)
{
	point average = { 0, 0 };
	for (int i = 0; i < sizeof(points); i++)
	{
		average.x += points[i].x;
		average.y += points[i].y;
		average.velx += points[i].velx;
		average.vely += points[i].vely;
	};
	average.x /= sizeof(points);
	average.y /= sizeof(points);
	average.velx /= sizeof(points);
	average.vely /= sizeof(points);
	return average;
}

double getarea(point* points)
{
	int xpoints = sizeof(points);
	double area = 0.0;
	int j = xpoints - 1;
	for (int i = 0; i < xpoints; i++)
	{
		area += (points[j].x + points[i].x) * (points[j].y - points[i].y);
		j = i;
	};
	return abs(area) / 2.0;
}


int main(int argc, char ** argv)
{
	int numpoints = 6;
	int numshapes = 2;
	point allpoints[6] = { {0, 0}, {1, 100}, {10, 0}, {10, 100}, { 100, 40}, {60, 3} };
	shape shapes[2] = { {0, 3}, {3, 3} };

	// variables
	bool quit = false;
	SDL_Event event;

	// init SDL

	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window * window = SDL_CreateWindow("Fishack",
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

		for (int shapi = 0; shapi < numshapes; shapi++)
		{
			for (int i = shapes[shapi].start + 1; i < shapes[shapi].size + shapes[shapi].start; i++)
			{
				SDL_RenderDrawLine(renderer, allpoints[i].x, allpoints[i].y, allpoints[i - 1].x, allpoints[i - 1].y);
			};
			SDL_RenderDrawLine(renderer, allpoints[shapes[shapi].start].x, allpoints[shapes[shapi].start].y, allpoints[shapes[shapi].start + shapes[shapi].size - 1].x, allpoints[shapes[shapi].start + shapes[shapi].size - 1].y);
		}

		SDL_RenderPresent(renderer);
	}

	// cleanup SDL

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
