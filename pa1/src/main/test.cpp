
#include "misc/global.hpp"

#include "data/data.hpp"
#include "parser/parser.hpp"
#include "fm/fm.hpp"

int main(int argc, char **argv) {
    list<int> l;
    l.emplace_front(3);
    auto a = l.begin();
    l.emplace_front(2);
    l.emplace_front(1);
    printf("size: %d\n", (int)l.size());
    for (auto it = l.begin(); it != l.end(); ++it) {
        printf("  %d\n", *it);
    }

    l.erase(l.begin());
    l.erase(a);
    printf("size: %d\n", (int)l.size());
    for (auto it = l.begin(); it != l.end(); ++it) {
        printf("  %d\n", *it);
    }


    return 0;
}

