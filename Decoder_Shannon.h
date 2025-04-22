/*
    Egor Shastin st129457@student.spbu.ru
    
    Header file for the DecoderShannon class, which inherits CoderShannon and implements text decoding.
*/


#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>

#include "dictionary.h"
#include "Encoder_Shannon.h"


#ifndef DECODER_SHANNON_H
#define DECODER_SHANNON_H


// ================== DecoderShannon ==================

class DecoderShannon : public CoderShannon {
private:
    
    bool is_num(const std::string elem) const; // Check is number or no
    
    std::string unescape(const std::string& str) const; // Check special symbol (\n, \t ...)
    
    void insertInDictionary(const std::vector<std::string>& input_correct, map<std::string, std::string>& dictionary); // Insert data in dectionary
    
    std::string decoder(const std::string input_file, map<std::string, std::string>& dictionary); // Decode text from txt file
    
public:
    
    void start_decoder(); // Start decoder shannon
};


#endif
