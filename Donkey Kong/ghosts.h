#pragma once
#include "library.h"
#include "board.h"
#include "utils.h"
#include "point.h"
#include "hazard.h"
#include "mario.h"


class ghosts : public hazard {


public:

	ghosts(int _x, int _y, Mario* mario, char symbol = 'x') : hazard(_x, _y, right, mario, symbol) {}
	virtual void move();


	void find_dir(int x, int y);

	bool search_dir(int x,int y);

};

 