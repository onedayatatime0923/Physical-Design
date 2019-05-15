#ifndef argparse__argparse_h
#define argparse__argparse_h

#include "misc/global.hpp"

class Argparse{
    public:
        Argparse                            (const int& argc, char **argv);
        float           apha                () const { return _apha; }
        const string&   inputBlock          () const { return _inputBlock; }
        const string&   inputNet            () const { return _inputNet; }
        const string&   output              () const { return _output; }
    private:

        vector<string> _tokenV;
        float          _apha;
        string         _inputBlock;
        string         _inputNet;
        string         _output;
};

#endif
