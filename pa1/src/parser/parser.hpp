
#ifndef parser__parser_h
#define parser__parser_h

#include "misc/global.hpp"
#include "data/data.hpp"



class Parser {
public:
    Parser(Data& data) : _data(data) {};
    void    parse   (const string& input);
private:
    Data& _data;
};

#endif
