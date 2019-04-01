
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
void Parser::dumpFile(int cut, const string& output) {
    FILE* file = fopen(output.c_str(), "w");

    vector<vector<Cell*>> groupCellVV;
    groupCellVV.resize(2);

    Cell* cellP;
    for (int i = 0; i < _data.cellPSize(); ++i) {
        cellP = _data.cellP(i);
        groupCellVV[cellP->finalBlock()].emplace_back(cellP);
    }

    fprintf(file, "Cutsize = %d\n", cut);
    for (int i = 0; i < (int)groupCellVV.size(); ++i) {
        fprintf(file, "G%d %d\n", i + 1, (int)groupCellVV[i].size());
        for (int j = 0; j < (int)groupCellVV[i].size(); ++j) {
            fprintf(file, "%s ", groupCellVV[i][j]->name().c_str());
        }
        fprintf(file, ";\n");
    }

};
