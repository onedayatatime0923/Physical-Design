
#include "misc/global.hpp"

#include "data/data.hpp"
#include "parser/parser.hpp"
#include "fm/fm.hpp"

int main(int argc, char **argv) {
    Data data;
    Parser parser(data);
    parser.parse(argv[1]);
    // data.print();

    FM fm(data);
    fm.solve();


    return 0;
}
