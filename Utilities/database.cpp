#include "database.h"
#include "tsymbolsmap.h"
#include <fstream>
#include <iostream>

// Constructors
DataBase::DataBase(const std::string& filepath) : enumerated(false), numberOfWords(0) 
{
    std::ifstream infile(filepath);
    if(infile.is_open()){
        std::cout << "|| Reading file: " << filepath << " | "; 
        RWord temp_buff;
        while(getline(infile, temp_buff)){
            RData.push_back(temp_buff);
            numberOfWords++;
        }
    }
    else{
        std::cout << "Could not access: " << filepath << std::endl;
        std::cout << "Exiting " << std::endl;
        exit(1);
    }

    std::cout << "Read " << numberOfWords << " words ||" << std::endl;
}

DataBase::~DataBase(){
    ;
}

const EnWord &DataBase::operator[](size_t n) const{
    if(enumerated && (n<numberOfWords)) return EData[n];
    else{
        std::cout << "Trying to access non-existing data, closing program" <<std::endl;
        exit(1);
    }
}


const RWord &DataBase::get_raw(size_t n) const{
    if(n<numberOfWords) return RData[n];
    else{
        std::cout << "Trying to access non-existing data, closing program" <<std::endl;
        exit(1);
    }
}

size_t DataBase::size() const{
    return numberOfWords;
}

bool DataBase::enumerate_db(const TSymbolsMap& m){
    for(size_t i = 0; i < this->size(); i++){
        EData.push_back(m.r2ew(RData[i]));
    }
    enumerated = true;
    return enumerated;
}