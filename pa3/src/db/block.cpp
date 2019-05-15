

#include "block.hpp"

void Block::print() {
    printf("  block id: %d\n", _id);
    printf("        name: %s\n", _name.c_str());
    printf("        size: %s\n", _size.str().c_str());
    printf("        dir: %d\n", _dir);
}
