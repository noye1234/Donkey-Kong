#include "betterGhosts.h"




void betterGhosts::move()
{
    if (isDead())
        return;
    if (check_hammer())
        return;

    int x_mario = get_mario_x();
    int y_mario = get_mario_y();
    int x = get_x();
    int y = get_y();

    if ((is_there_ladder(x + get_dir(), y) && dir_y < 0)) {
        if (!is_there_ladder(x + get_dir(), y-1)) {
            move_up_or_down(x, y, true);
            move_up_or_down(get_x(), get_y(), false);
            dir_y = 0;
            is_claim_up = false;
            if (x_mario > get_x())
                set_dir(right);
            else set_dir(left);
        }
        else {
            move_up_or_down(x, y, true);

            if (is_floor(get_x(), get_y() - 1)) {
                move_up_or_down(get_x(), get_y(), false);
                move_up_or_down(get_x(), get_y(), false);
                dir_y = 0;
                is_claim_up = false;
                if (x_mario > get_x())
                    set_dir(right);
                else set_dir(left);

            }
            else is_claim_up = true;
        }

    }
    else if (((is_there_ladder(x, y - 1) && dir_y < 0)) || is_claim_up) {

        move_up_or_down(x, y, false);
        if (is_floor(get_x(), get_y() + dir_y)) {
            move_up_or_down(get_x(), get_y(), false);
            move_up_or_down(get_x(), get_y(), false);
            dir_y = 0;
            is_claim_up = false;
            if (x_mario > x)
                set_dir(right);
            else set_dir(left);
        }
        else is_claim_up = true;
    }
    else if ((is_there_ladder(x, y + 2) && dir_y > 0) || is_claim_down) {

        set_dir(0);
        if (!is_claim_down) {
            dir_y++;
            move_up_or_down(x, y, false);
            dir_y--;
        }
        else
            move_up_or_down(x, y, false);

        if (is_floor(get_x(), get_y() + 1)) {
            dir_y = 0;
            is_claim_down = false;
            if (x_mario > get_x())
                set_dir(right);
            else set_dir(left);
        }
        else is_claim_down = true;

    }
    else {
        if (y_mario > y)
            dir_y = 1;
        else if (y_mario < y) dir_y = -1;
        else dir_y = 0;

        search_dir(x, y);
        hazard::move();
    }
    if (get_char(get_x() + get_dir(), get_y()) == ghost || get_char(get_x() + get_dir(), get_y()) == 'X') {
        set_dir(-get_dir());
    }
}

void betterGhosts::move_up_or_down(int x, int y, bool is_first_floor)
{
    
    char ch= get_backgroundchar();
    if (good_backgroundchar())
        set_char(ch, x, y);
    if (is_first_floor) {
        x += get_dir();
        set_x(x);
    }

    if ((y + dir_y) <= START_Y || get_char(x, y + 3* dir_y)==ghost)
        return;
    y += dir_y;
    if (check_mario_dir() )
        return;
    set_backgroundchar(get_char(x, y));

    set_char(symbol, x, y);

    set_y(y);

}

void betterGhosts::restart()
{
    ghosts::restart();
    dir_y = 0;
    is_claim_up = false;
    is_claim_down = false;
}
