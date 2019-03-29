
#include "parser.hpp"

void Parser::parse(const string& input) {
    FILE* file = fopen(input.c_str(), "r");
    char buffer[1024];
    string bufferS;
    // printf("%s\n", input.c_str());
    fscanf(file, "%f\n", &_data.balanceDegree());
    // printf("%f\n", _data.balanceDegree());
    Net* netP;
    Cell* cellP;
    bool netB;
    while(fscanf(file, "%s \n", buffer) > 0) {
        bufferS = string(buffer);
        if (bufferS == "NET") {
            netB = true;
        }
        else if (netB) {
            netB = false;
            netP = _data.net(bufferS);
        }else if (bufferS == ";") {
            continue;
        }else {
            cellP = _data.cell(bufferS);
            netP->addCell(cellP);
            cellP->addNet(netP);
        }
        // printf("%s\n", buffer);
    }
    _data.spread();
};
