
#include "parser.hpp"

void Parser::parse(const string& block, const string& net, DB& db) {
    FILE* file = fopen(block.c_str(), "r");
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
    file = fopen(net.c_str(), "r");
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
};
