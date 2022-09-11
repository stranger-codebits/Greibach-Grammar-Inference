#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>

// The struct of the raw data stored
typedef std::string RWord;                          // Raw word
typedef std::vector<RWord> RawData;

typedef std::vector<unsigned int> EnWord;           // Enumerated word
typedef std::vector<EnWord> EnumeratedData; 

class TSymbolsMap;                                  // Forward declaration of symbols map

// This class will hold enumerated data
class DataBase
{
    public:
        DataBase(const std::string& filepath);      // Store contents of a file
        ~DataBase();                                // Free any allocated memory

        size_t size() const;                        // Number of instances in the database

        bool enumerate_db(const TSymbolsMap& m);    // Enumerate all data words

        const EnWord &operator[](size_t n) const;   // Get the n-th enumerated dataword
        const RWord &get_raw(size_t n) const;       // Get the n-th raw data word

    private:
        size_t numberOfWords;                       // Words parsed
        bool enumerated;                            // If the data are enumerated

        RawData RData;                              // Raw Data
        EnumeratedData EData;                       // Enumerated Data
};


#endif //DATABASE_H