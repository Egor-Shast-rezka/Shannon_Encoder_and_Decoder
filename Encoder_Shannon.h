/*
    Egor Shastin st129457@student.spbu.ru
    
    Implementation of methods of the CoderShannon class for encoding text using the Shannon-Fano algorithm.
*/


#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>


#ifndef ENCODER_SHANNON_H
#define ENCODER_SHANNON_H


// ================== CoderShannon ==================

class CoderShannon {
protected:
    
    float sum_dict(const std::vector<std::pair<std::string, float>>& dict); // Sum all second value in dictionarry
    
    void Split(const std::vector<std::pair<std::string, float>>& dict,
        std::vector<std::pair<std::string, std::string>>& code_result,
        int start_i = 0); // Get each symbol unique num
    
    virtual std::string read_file(const std::string& filename); // Read file from txt file
    
    void write_file(const std::string& filename, const std::string& content); // Write file in txt file
    
    std::string checkAnswerUser(std::string call, std::vector<std::string> values); // Check user answer
    
    std::string escape_char(const std::string& ch); // Check special symbol (\n, \t ...)
    
    std::vector<std::string> utf8_split(const std::string& str); // The function splits a UTF-8 encoded string into individual characters.
    
public:
    
    void start_encoder(); // Start encoder shannon
};


#endif
