

#include "terminal.hpp"

void Terminal::print() {
    printf("  terminal id: %d\n", _id);
    printf("           name: %s\n", _name.c_str());
    printf("           coor: %s\n", _coor.str().c_str());
}
