#include "tsymbolsmap.h"
#include <algorithm>
#include <iostream>

TSymbolsMap::TSymbolsMap()
{
    ;
}

TSymbolsMap::~TSymbolsMap()
{
    ;
}

bool TSymbolsMap::ParseData(const DataBase &d){
    size_t nd = d.size();

    RWord temp_word;
    // Iterate over data
    for(size_t i = 0; i < nd; i++){
        temp_word = d.get_raw(i);
        for(auto c : temp_word){
            // Char not found
            if(!(std::find(terminals.begin(), terminals.end(), c)!=terminals.end())){
                terminals.push_back(c);
            }
        }
    }

    return true;
}

unsigned int TSymbolsMap::c2ui(char c) const{
    auto itr = std::find(terminals.begin(), terminals.end(), c);
    if(itr != terminals.end()) return std::distance(terminals.begin(), itr) + 1; // element found
    else{
        std::cout << "Element " << c << "not enumerable, exiting" << std::endl;
        exit(1);
    }
}

EnWord TSymbolsMap::r2ew(const RWord& w) const{
    EnWord ret;
    for(auto c : w) ret.push_back(c2ui(c));
    return ret;
}


unsigned int TSymbolsMap::max_c() const{
    return terminals.size();
}