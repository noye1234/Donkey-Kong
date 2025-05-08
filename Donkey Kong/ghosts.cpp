#include "utils.h"
#include "ghosts.h"
#include "mario.h"


// Handle barrel movement logic
void ghosts::move() {
    find_dir(get_x(), get_y());
    hazard::move();
    if (get_char(get_x() + get_dir(), get_y()) == ghost) {
        set_dir(-get_dir());
    }
}

// Find the new direction of the barrel
void ghosts::find_dir(int x, int y) {
    if (check_hammer())
        return;
    if (!search_dir(x, y) && !biased_random())
        set_dir(-get_dir());
}
 

bool ghosts::search_dir(int x,int y) {
    char ch, ch2;

    ch = get_char(x, y + 1);
    if (is_falling(ch)) {
        if (check_mario(x, y + 1))
            hurt_mario();
        set_dir(stop_move);
        ch = get_char(get_x(), get_y() + 2);
        if (!is_falling(ch)) {
            set_dir(right);
        }
        return true;
    }
    ch = get_char(x + get_dir(), y + 1);
    ch2 = get_char(x + get_dir(), y);
    if (ch2 == ghost || ch2 == wall || ch2 == Bghost || (ch != leftFloor && ch != rightFloor && ch != middleFloor)) {
        set_dir(-get_dir());
        return true;
    }
    return false;
}
