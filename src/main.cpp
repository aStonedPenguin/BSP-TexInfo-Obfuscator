#include <iostream>
#include <fstream>
#include <cstring>
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

        BSPFile bsp;
        bsp.load(file);
        
        ofstream out(strcat(fileName, ".obfuscated"), ios::binary);
        file.seekg(0);
        out << file.rdbuf();

        lump_t brushsideLump = bsp.getHeader().lumps[LUMP_BRUSHSIDES_INDEX];

        out.seekp(brushsideLump.fileofs);
        short firstIndex(0); // The index 0 generally corresponds to the nodraw texture
        for (int i = 0; i < brushsideLump.filelen/sizeof(dbrushside_t); i++){
            out.seekp(sizeof(short), ios::cur); // ignore planenum

            out.write(reinterpret_cast<char*>(&firstIndex), sizeof(short)); // texinfo

            out.seekp(sizeof(short), ios::cur); // ignore dispinfo
            out.seekp(sizeof(short), ios::cur); // ignore bevel
        }

        // Free memory
        out.close();
        file.close();

        cout << "Done !" << endl;
    }else{
        cout << "Failed to open the file ! (" << fileName << ")" << endl;
    }

    return 0;
}
