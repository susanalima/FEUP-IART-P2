#include "Utils.h"

#include <random>
#include <iostream>
#include <chrono>

int random(int min, int max)
{
	std::mt19937::result_type seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	std::mt19937 rng(seed);
	std::uniform_int_distribution<int> gen(min, max);
	int r = gen(rng);
	return r;
}