#pragma once

#include <time.h>
#include <random>

FishackBegin

// https://youtu.be/oW6iuFbwPDg?t=277
void RandomSeed()
{
	std::random_device rd;
	std::uniform_int_distribution<int> random;
	srand( (unsigned int)(random(rd) * time(NULL)) );
}

FishackEnd
