#include "stdafx.h"
#include "memory.h"


uintptr_t FindDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets) {
	uintptr_t addr = ptr;
	for (unsigned int i = 0; i < offsets.size(); ++i) {
		addr = *(uintptr_t*)addr;
		addr += offsets[i];
	}
	return addr;
}

void Player::load_properties() {
	if (FindDMAAddy(this->base_ptr, { 0x0 }) != 0) {
		this->name = (char*)FindDMAAddy(this->base_ptr, { 0xfc, 0x44, 0x0 });
		// RESOURCES
		this->food = *(float*)FindDMAAddy(this->base_ptr, { 0xfc, 0x50, 0x0 });
		this->wood = *(float*)FindDMAAddy(this->base_ptr, { 0xfc, 0x50, 0x4 });
		this->stone = *(float*)FindDMAAddy(this->base_ptr, { 0xfc, 0x50, 0x8 });
		this->gold = *(float*)FindDMAAddy(this->base_ptr, { 0xfc, 0x50, 0xC });
		// GAME PLAY
		this->current_pop = *(float*)FindDMAAddy(this->base_ptr, { 0x2c });
		this->current_villagers = *(float*)FindDMAAddy(this->base_ptr, { 0x94 });
		this->current_soldiers = (float)this->current_pop - this->current_villagers;
		this->current_age = *(float*)FindDMAAddy(this->base_ptr, { 0x18 });

		// BUILDING
		this->building_count = *(int*)FindDMAAddy(this->base_ptr, { 0xfc, 0x40, 0x8 });
		for (unsigned int i = 0; i < this->building_count; i++) {
			uintptr_t tmp = FindDMAAddy(this->base_ptr, { 0xfc, 0x40, 0xc, i * 0x4, 0x10, 0x48 });
			buildings[i].name = tmp?(char*)tmp:"";
		}
	}
	else {
		this->base_ptr = 0;
	}
}


