#pragma once
#include "library.h"

void gotoxy(int x, int y);

void ShowConsoleCursor(bool showFlag);

class barrels;
class ghosts;

char readCharFromConsole(int x, int y);

bool is_falling(const char ch);

bool is_there_ladder(int x, int y);

void off();

int biased_random();

bool is_it_special_char(char ch);

bool is_floor(int x, int y);

bool is_save();


void clearScreen();

char waitForValidInput(const std::vector<char>& validKeys);


//new2
bool get_silence();

long get_random_seed(); 