#include <iostream>
#include <fstream>
#include <cstring>
#include <map>
#include "BSPFile.h"

using namespace std;

// Get all texinfo and link them by flag
//


int main(int argc, char **argv) {
    if (argc <= 1){
        cout << "You must specify the location of the BSP file !" << endl;
        return 0;
    }

    char* fileName = argv[1];
    ifstream file(fileName, ios::binary);

    if (file.is_open()){
        cout << "Starting obfuscation... (" << fileName << ")" << endl;

        BSPFile bsp(file);
        vector<dbrushside_t> brushSides = bsp.getBrushSides();
        vector<texinfo_t> texInfos = bsp.getTexInfos();

        map<int, int> texInfosByFlag;
        for(auto &side : brushSides){
            int texInfo = side.texinfo;
            texInfosByFlag[texInfos[texInfo].flags] = texInfo;
        }

        // Copy the file
        ofstream out(strcat(fileName, ".obfuscated"), ios::binary);
        out << file.rdbuf();

        lump_t brushsideHeader = bsp.getHeader().lumps[LUMP_BRUSHSIDES_INDEX];

        out.seekp(brushsideHeader.fileofs);
        for(auto side : brushSides){
            texinfo_t texInfo = texInfos[side.texinfo];
            side.texinfo = texInfosByFlag[texInfo.flags];

            out.write(reinterpret_cast<char*>(&side), sizeof(dbrushside_t));
        }

        out.close();
        file.close();

        cout << "Done !" << endl;
    }else{
        cout << "Failed to open the file ! (" << fileName << ")" << endl;
    }

    return 0;
}
