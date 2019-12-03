#pragma once
#include "stdafx.h"
#include <vector>

class Building {
public:
	char* name;
};

class Player {
public:
	uintptr_t base_ptr;
	float food, wood, gold, stone;
	float current_pop, current_villagers, current_soldiers;
	float current_age;
	int building_count;
	Building buildings[1000];
	char* name;

	void load_properties();
};



uintptr_t FindDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets);