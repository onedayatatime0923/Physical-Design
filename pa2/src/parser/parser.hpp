
#include "misc/global.hpp"
#include "io.hpp"
#include "routingdb.hpp"
#include "tree.hpp"
#include "db/db.hpp"

#ifndef parser__parser_h
#define parser__parser_h

namespace SelfParser {

    ifstream inputFile;
    ofstream outputFile;

    RoutingDB db;

    Point origin, tileSize;

    void ShowSyntax() {
        cout << "syntax: " << endl;
        cout << "./gr [input_file] [output_file] " << endl;
        cout << endl;

        cout << "example: " << endl; 
        cout << "./gr input/adaptec1.capo70.2d.35.50.90.gr a1.out" << endl;

        cout << endl;
    }

    void HandleArgument(const int argc, char** argv) {
        if (argc < 3)
        { ShowSyntax(); exit(1); }

        int arg_no = 1;

        /* input file */
        inputFile.open(argv[arg_no], ios::in);
        if (!inputFile)
        {
        cerr << "Could not open input file: " << argv[arg_no] << endl;
        exit(1);
        }

        arg_no++;

        /* output file */
        outputFile.open(argv[arg_no], ios::out);
        if (!outputFile)
        {
        cerr << "Could not open output file: " << argv[arg_no] << endl;
        exit(1);
        }

        arg_no++;
    }


    int parse(int argc, char** argv, DB& selfDB) {
        HandleArgument(argc, argv); 


        {
            Parser parser;
            parser.ReadISPD(inputFile);
        }

        CapacityTable& capacityTable = selfDB.capacityTable();

        selfDB.setSize(db.GetHoriGlobalTileNo(), db.GetVertiGlobalTileNo());

        capacityTable.setHor(db.GetLayerHoriCapacity(0) / 2);
        capacityTable.setVer(db.GetLayerVertiCapacity(1) / 2);


        origin.set(db.GetLowerLeftX(), db.GetLowerLeftY());
        tileSize.set(db.GetTileWidth(), db.GetTileHeight());

        int layer, capacity;
        Point p1, p2;
        Point pSmall, pBig;
        for (int i = 0; i < db.GetCapacityAdjustNo(); i++) {
            CapacityAdjust& ca = db.GetCapacityAdjust(i);
            p1.set(ca.GetGx1(), ca.GetGy1());
            p2.set(ca.GetGx2(), ca.GetGy2());
            pSmall = min(p1, p2);
            pBig = max(p1, p2);
            assert(ca.GetLayer1() == ca.GetLayer2());
            layer = ca.GetLayer1();
            capacity = ca.GetReduceCapacity() / 2;
            for (Point p = pSmall; p < pBig;) {
                if (layer == 1) {
                    capacityTable.setHor(p, capacity);
                    ++p[0];
                }
                else if (layer == 2) {
                    capacityTable.setVer(p, capacity);
                    ++p[1];
                }
                else assert(false);
            }
        }


        {
            RoutingTree tree;
            tree.MinimumSpanningTreeConstruction();
        }

        for (int i = 0; i < db.GetNetNo(); i++) {
            Net& n = db.GetNetByPosition(i);

            selfDB.pushNet(i, n.GetUid(), n.GetName());

            for (int i = 0; i < n.GetSubNetNo(); i++) {
                SubNet& e = n.GetSubNet(i);
                selfDB.pushPoint(e.GetSourcePin().GetGx(), e.GetSourcePin().GetGy(),
                                e.GetTargetPin().GetGx(), e.GetTargetPin().GetGy());
            }
        }

        return 0;
    }
    int dumpFile(int argc, char** argv, DB& selfDB) {
        FILE* output = fopen(argv[2], "w");
        for (int i = 0; i < selfDB.netSize(); ++i) {
            set<Segment3D>& segmentS = selfDB.net(i).segmentS();
            fprintf(output, "%s %d %lu\n", selfDB.net(i).name().c_str(), selfDB.net(i).id(), segmentS.size());
            for (auto it = segmentS.begin(); it != segmentS.end(); ++it) {
                const Point3D& p1 = it->point1();
                const Point3D& p2 = it->point2();
                fprintf(output, "(%d,%d,%d)-(%d,%d,%d)\n", db.CalCenterX(p1[0]), db.CalCenterY(p1[1]), p1[2], db.CalCenterX(p2[0]), db.CalCenterY(p2[1]), p2[2]);
            }
            fprintf(output, "!\n");
        }
        return 0;
    }
    void verify(int argc, char** argv) {
        cout << "[Verify]" << endl;
        char cmd[100];

        sprintf(cmd, "./tool/eval.pl %s %s", argv[1], argv[2]);
        cout << cmd << endl;
        system(cmd);
    }


}


#endif
