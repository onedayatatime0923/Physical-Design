

#include "net.hpp"

void Net::print() {
    printf("  net id: %d\n", _id);
    printf("    block: %lu\n", _blockPV.size());
    printf("      ");
    for (int i = 0; i < (int)_blockPV.size(); ++i) {
        if (i % 5 == 4) printf("\n      ");
        printf(" %s", _blockPV[i]->name().c_str());
    }
    printf("\n");
    printf("    terminal: %lu\n", _terminalPV.size());
    printf("      ");
    for (int i = 0; i < (int)_terminalPV.size(); ++i) {
        if (i % 5 == 4) printf("\n      ");
        printf(" %s", _terminalPV[i]->name().c_str());
    }
    printf("\n");
}
