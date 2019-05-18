
#include "parser.hpp"

void Parser::parse(DB& db) {
    FILE* file = fopen(_opt.inputBlock().c_str(), "r");
    char buffer[1024];
    string bufferS;
    int nBlock, nTerminal, nNet, nDegree, x, y;
    fscanf(file, "Outline: %d %d\n", &x, &y);
    db.setSize(x,y);

    fscanf(file, "NumBlocks: %d\n", &nBlock);
    fscanf(file, "NumTerminals: %d\n", &nTerminal);
    for (int i = 0; i < nBlock; ++i) {
        fscanf(file, "%s %d %d ", buffer, &x, &y);
        bufferS = string(buffer);
        db.pushBlock(bufferS, x, y);
    }
    for (int i = 0; i < nTerminal; ++i) {
        fscanf(file, "%s terminal %d %d ", buffer, &x, &y);
        bufferS = string(buffer);
        db.pushTerminal(bufferS, x, y);
    }
    fclose(file);
    file = fopen(_opt.inputNet().c_str(), "r");
    fscanf(file, "NumNets: %d\n", &nNet);
    for (int i = 0; i < nNet; ++i) {
        db.pushNet();
        fscanf(file, "Net Degree: %d\n", &nDegree);
        for (int j = 0; j < nDegree; ++j) {
            fscanf(file, "%s ", buffer);
            bufferS = string(buffer);
            db.pushPin(bufferS);
        }
    }
    db.setApha(_opt.apha());
};
void Parser::dumpFile(DB& db, Bstar& bstar, double time) {
    FILE* file = fopen(_opt.output().c_str(), "w");
    const Point& size = bstar.finalSize();
    int wireLength = bstar.finalWireLength();
    int cost = (int)((_opt.apha() * size[0] * size[1]) + ((1 - _opt.apha()) * wireLength));
    int area = (int)((_opt.apha() * size[0] * size[1]));
    fprintf(file, "%d\n", cost);
    fprintf(file, "%d\n", wireLength);
    fprintf(file, "%d\n", area);
    fprintf(file, "%d %d\n", size[0], size[1]);
    fprintf(file, "%lf\n", time);
    for (int i = 0; i < db.blockSize(); ++i) {
        const Rect& location = bstar.finalLocation(i);
        fprintf(file, "%s %d %d %d %d\n", db.block(i).name().c_str(), location[0], location[1], location[2], location[3]);
    }

}
