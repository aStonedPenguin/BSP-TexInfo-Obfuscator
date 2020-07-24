#include <iostream>
#include <fstream>
#include <cstring>
#include <map>
#include "BSPFile.h"

using namespace std;

int main(int argc, char **argv) {
    if (argc <= 1){
        cout << "You must specify the location of the BSP file !" << endl;
        return 0;
    }

    char* fileName = argv[1];
    ifstream file(fileName, ios::binary);

    if (file.is_open()){
        cout << "Starting obfuscation... (" << fileName << ")" << endl;

        // Read the BSP file
        BSPFile* bsp;
        try{
            bsp = new BSPFile(file);
        }catch(std::bad_alloc& e){
            cerr << "Failed to read the file, is this a valid BSP file ?" << endl;
            return 0;
        }

        vector<dbrushside_t> brushSides = bsp->getBrushSides();
        vector<texinfo_t> texInfos = bsp->getTexInfos();

        // Get all different surface flags and and pick a corresponding texinfo
        map<int, int> texByFlags;
        for(auto &side : brushSides)
            texByFlags[texInfos[side.texinfo].flags] = side.texinfo;

        // Copy the file
        ofstream out(strcat(fileName, ".obfuscated"), ios::binary);
        if (!out.is_open()){
            cerr << "Failed to create a new file" << endl;
            return 0;
        }
        file.seekg(0);
        out << file.rdbuf();

        // Write the new texinfos on the brushside lump
        out.seekp(bsp->getHeader().lumps[LUMP_BRUSHSIDES_INDEX].fileofs);
        for(auto &side : brushSides){
            texinfo_t& texInfo = texInfos[side.texinfo];
            side.texinfo = texByFlags[texInfo.flags];

            out.write(reinterpret_cast<char*>(&side), sizeof(dbrushside_t));
        }

        // Close the streams
        delete bsp;
        out.close();
        file.close();

        cout << "Done !" << endl;
    }else{
        cerr << "Failed to open the file ! (" << fileName << ")" << endl;
    }

    return 0;
}
