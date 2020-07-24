#include "BSPFile.h"

BSPFile::BSPFile(std::ifstream& infile){
    load(infile);
}

void BSPFile::load(std::ifstream& infile){
    std::streampos old = infile.tellg();

    infile.seekg(0);
    infile.read(reinterpret_cast<char*>(&header), sizeof(dheader_t));

    load_lump(infile, (uint8_t) LUMP_TEXINFO_INDEX, texInfos); // Load texinfo
    load_lump(infile, (uint8_t) LUMP_BRUSHSIDES_INDEX, brushSides); // Load brushsides

    infile.seekg(old);
}

template<typename type>
void BSPFile::load_lump(std::ifstream& infile, uint8_t index, std::vector<type>& out){
    lump_t& lump = header.lumps[index];
    out.resize(lump.filelen/sizeof(type));

    infile.seekg(lump.fileofs);
    infile.read(reinterpret_cast<char*>(out.data()), lump.filelen);
}