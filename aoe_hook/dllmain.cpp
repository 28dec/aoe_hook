#include "stdafx.h"
#include "console.h"
#include "memory.h"
#include <vector>
#include <ctime>
#define show_console 0 //1 = show console ~ 0 = don't show console


typedef void(__thiscall* _create_msg)(uintptr_t*, const char*, const char*, int);

_create_msg create_msg;
uintptr_t** chat_class;

uintptr_t module_base = (uintptr_t)GetModuleHandle(NULL);
uintptr_t imba_base = (uintptr_t)GetModuleHandle("imba.dll");
uintptr_t p1_base_ptr = (uintptr_t)(imba_base + 0x1A434);

Player player[9];

const int IN_ROOM = 1;
const int IN_GAME = 2;
int GAME_STATUS = 0;
char last_msg[200] = "";
clock_t last_time_msg_sent = std::clock();
int time_elapsed() {
	clock_t current_time = std::clock();
	return ((current_time - last_time_msg_sent) / CLOCKS_PER_SEC);
}

void send_msg(const char* author, const char* msg) {
	if ((strcmp(msg, last_msg) && time_elapsed() > 3) || (!strcmp(msg, last_msg) && time_elapsed()>9)) {
		create_msg(*chat_class, author, msg, 1);
		last_time_msg_sent= std::clock();
		strcpy(last_msg, msg);
	}
}

DWORD WINAPI init_gameplay(LPVOID param) {
	for (int i = 1; i < 9; i++) {
		player[i].base_ptr = (uintptr_t)(imba_base + 0x1A430 + 0x4 * i);
		player[i].load_properties();
	}
	if (player[1].base_ptr == 0) {
		GAME_STATUS = IN_ROOM;
	}
	else {
		GAME_STATUS = IN_GAME;
	}
	return 0;
}

void d3kt_validity(Player p) {
	char msg_buffer[200];
	if (p.current_age < 3 && p.current_soldiers > 1) {
		std::cout << p.name << " age " << p.current_age << std::endl;
		sprintf_s(msg_buffer, 200, "[%s] xin %d don vi chien dau o doi %d.", p.name, (int)p.current_soldiers, (int)p.current_age);
		send_msg("GM", msg_buffer);
	}
	for (int j = 0; j < p.building_count; j++) {
		if (strstr(p.buildings[j].name, "Twr")) {
			sprintf_s(msg_buffer, 200, "phat hien [%s] xay choi`.", p.name);
			send_msg("GM", msg_buffer);
		}
		if (strstr(p.buildings[j].name, "Wall")) {
			sprintf_s(msg_buffer, 200, "phat hien [%s] xay thanh`.", p.name);
			send_msg("GM", msg_buffer);
		}
	}
}

DWORD WINAPI check_d3kt(LPVOID param) {
	while (true) {
		Sleep(50);
		init_gameplay(0);
		if (GAME_STATUS != IN_GAME) continue;
		for (int i = 1; i < 9; i++) {
			d3kt_validity(player[i]);
		}
	}
}

DWORD WINAPI MainThread(LPVOID param) {
	create_msg = (_create_msg)(module_base + 0x1CEA0);
	while (!GetAsyncKeyState(VK_END) & 1) {
		Sleep(50);
		chat_class = (uintptr_t**)(module_base + 0x185DF0);
		if (GetAsyncKeyState(VK_INSERT) & 1) {
			char buffer[200];
			sprintf_s(buffer, 200, "current age -> %d, current soldiers -> %d.", (int)player[1].current_age, (int)player[1].current_soldiers);
			create_msg(*chat_class, (const char*)player[1].name, buffer, 1);
		}
	}
	FreeLibraryAndExitThread((HMODULE)param, 0);
	return 0;
}


BOOL APIENTRY DllMain(HMODULE hModule,
	uintptr_t  ul_reason_for_call,
	LPVOID lpReserved
) {
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
		CreateThread(0, 0, MainThread, hModule, 0, 0);
		CreateThread(0, 0, check_d3kt, hModule, 0, 0);
		if (show_console)console(hModule);
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
