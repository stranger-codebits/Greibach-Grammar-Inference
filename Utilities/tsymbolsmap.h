#ifndef TSYMBOLSMAP_H
#define TSYMBOLSMAP_H

#include "database.h"
#include <vector>

typedef std::vector<char> CharMap; 

// This class holds all terminal symbols
class TSymbolsMap
{
    public:
        TSymbolsMap();                          // Constructor
        ~TSymbolsMap();                         // Destructor

        bool ParseData(const DataBase &d);      // Parse a database and add all symbols to enumeration map

        unsigned int c2ui(char c) const;        // Transform a char to enumerated form (char to unsigned int)
        EnWord r2ew(const RWord& w) const;      // Transform a string vector<inr> (raw word to enumerated form)

        unsigned int max_c() const;             // Max enumerated character

    private:
        CharMap terminals;
};


#endif // TSYMBOLSMAP_H