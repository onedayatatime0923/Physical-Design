#include "argparse.hpp"


Argparse::Argparse(const int& argc, char** argv) {
    for (int i=1; i < argc; ++i)
        _tokenV.push_back(string(argv[i]));
    _apha = stof(_tokenV[0]);
    _inputBlock = _tokenV[1];
    _inputNet= _tokenV[2];
    _output = _tokenV[3];
}

