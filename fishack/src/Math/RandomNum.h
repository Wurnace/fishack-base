#pragma once

// https://youtu.be/oW6iuFbwPDg?t=277

#include <random>
// C++ standard library - could use this instead for RNG, not just for seed. 
#include <time.h>
// time() returns seconds passed since 00:00:00 GMT, Jan 1, 1970. 

FishackBegin

/*
	Seeds The RNG using Time and a random number. 
*/
void RandomSeed()
{
	std::random_device rd;
	std::uniform_int_distribution<int> random;
	srand( (unsigned int)(random(rd) * time(NULL)) );
}

FishackEnd
