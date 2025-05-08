#pragma once

#include "utils.h"
#include "ghosts.h"
#include "mario.h"


class betterGhosts : public ghosts {
	int dir_y = 0;
	char symbol = 'X';
	int count_check = 0;
	bool is_claim_up= false;
	bool is_claim_down= false;
public:

	betterGhosts(int _x, int _y, Mario* mario,char symbol ='X') :ghosts(_x, _y, mario, symbol) {};
	virtual void move();
	void move_up_or_down(int x, int y,bool is_first_floor);
	virtual void restart();
};
