#pragma once
#include "library.h"
#include "board.h"
#include "utils.h"
#include "point.h"
class Mario;
class Game;


class hazard {
	int start_x, start_y, start_dir;
	int x, y;
	int dir;
	char save_symbol;
	char symbol;
	Mario* mario;
	char backgroundchar = ' ';

	bool is_dead = false;

	friend class Game;
public:
	enum { left = -1, right = 1, delete_ = 0, stop_move = 0 };
	hazard(int _x, int _y, int _dir, Mario* mario_, char symbol_ )
		:start_x(_x), start_y(_y), x(_x), y(_y), start_dir(_dir),
		 dir(_dir), symbol(symbol_), save_symbol(symbol_), mario(mario_) {

	}

	virtual void restart();
	bool check_mario(const int _x, const int _y)const;
	virtual void move();

	int get_x()const { return x; }
	int get_y() const { return y; }
	int get_dir() const { return dir; }
	void set_x(int x_) { x = x_; }
	void set_y(int y_) { y = y_; }
	void set_dir(int dir_) { dir = dir_; }

	void hurt_mario();
	bool check_mario_dir();

	void move_dir(int x, bool is_there_ladder);

	int check_dir(int x, int y, int dir = right);

	void set_start_dir(int dir_) { start_dir = dir_; }

	void dissapir(); 
	bool check_hammer();

	int get_mario_x();
	int get_mario_y();

	int get_symbol() {return symbol ; }

	char get_backgroundchar() { return backgroundchar; }
	void set_backgroundchar(char new_) { backgroundchar =new_; }
	bool good_backgroundchar();

	bool isDead() { return is_dead; }

	char get_char(int x, int y);
	void set_char(char ch, int x, int y);

	virtual ~hazard() {};
};