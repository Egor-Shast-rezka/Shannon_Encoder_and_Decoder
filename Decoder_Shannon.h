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
    
    bool is_num(const std::string elem); // Check is number or no
    
    std::string unescape(const std::string& str); // Check special symbol (\n, \t ...)
    
    std::string read_file(const std::string& filename) override; // Read file from txt file
    
    std::string decoder(std::string input_file, map<std::string, std::string> dictionary); // Decode text from txt file
    
public:
    
    void start_decoder(); // Start decoder shannon
};


#endif
