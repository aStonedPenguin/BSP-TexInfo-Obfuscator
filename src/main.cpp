/*
    Created by Feeps on 24/07/2020.
*/

#include <iostream>
#include <fstream>
#include <cstring>
#include <map>
#include "BSPFile.h"

using namespace std;

void obfuscateFlags(BSPFile* bsp, ofstream& out);
void obfuscateNoDraw(BSPFile* bsp, ofstream& out);

int main(int argc, char **argv) {
    if (argc <= 2){
        cout << "Incorrect usage: You must specify an obfuscation method and the location of a BSP file !" << endl;
        cout << "Usage: " << endl;
        cout << "\t" << argv[0] << " -flags <file>" << endl;
        cout << "\t" << argv[0] << " -nodraw <file>" << endl;
        return 0;
    }

    char* method = argv[1];
    if(strcmp(method, "-flags")  != 0 && strcmp(method, "-nodraw") != 0){
        cerr << "You specified an invalid method !" << endl;
        return 0;
    }

    char* fileName = argv[2];
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

        // Copy the file
        ofstream out(strcat(fileName, ".obfuscated"), ios::binary);
        if (!out.is_open()){
            cerr << "Failed to create a new file" << endl;
            return 0;
        }
        file.seekg(0);
        out << file.rdbuf();

        // Obfuscate
        if (strcmp(method, "-flags") == 0)
            obfuscateFlags(bsp, out);
        else
            obfuscateNoDraw(bsp, out);

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

void obfuscateFlags(BSPFile* bsp, ofstream& out){
    vector<dbrushside_t> brushSides = bsp->getBrushSide();
    vector<texinfo_t> texInfos = bsp->getTexInfo();

    // Get all different surface flags and and pick a corresponding texinfo
    map<int, int> texByFlags;
    for(auto &side : brushSides)
        texByFlags[texInfos[side.texinfo].flags] = side.texinfo;

    out.seekp(bsp->getHeader().lumps[LUMP_BRUSHSIDES_INDEX].fileofs);
    for(auto &side : brushSides){
        texinfo_t& texInfo = texInfos[side.texinfo];
        side.texinfo = texByFlags[texInfo.flags]; // Write the new texinfos on the brushside lump

        out.write(reinterpret_cast<char*>(&side), sizeof(dbrushside_t));
    }
}

void obfuscateNoDraw(BSPFile* bsp, ofstream& out){
    vector<dbrushside_t> brushSide = bsp->getBrushSide();
    vector<string> texDataStringData = bsp->getTexDataStringData();

    for(int i = 0; i < texDataStringData.size(); i++)
        cout << i << ": " << texDataStringData[i] << endl; 

    unsigned int sel;
    do{
        cout << "Choose a valid texture: ";
        cin >> sel;
    }while(sel >= texDataStringData.size());

    // Find a texinfo that have this texture
    vector<texinfo_t> texinfos = bsp->getTexInfo();
    vector<dtexdata_t> texdatas = bsp->getTexData();
    int texIndex;
    for(int i = 0; i < texdatas.size(); i++){
        texinfo_t tex = texinfos[i];
        dtexdata_t data = texdatas[tex.texdata];
        if(data.nameStringTableID == sel){
            texIndex = i;
            break;
        }
    }

    out.seekp(bsp->getHeader().lumps[LUMP_BRUSHSIDES_INDEX].fileofs);
    for(auto &side : brushSide){
        side.texinfo = texIndex;
        out.write(reinterpret_cast<char*>(&side), sizeof(dbrushside_t));
    }
} 