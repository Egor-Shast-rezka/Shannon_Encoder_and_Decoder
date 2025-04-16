/*
    Egor Shastin st129457@student.spbu.ru
    
    
*/


#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>


#ifndef ENCODER_SHANNON_H
#define ENCODER_SHANNON_H


// ================== CoderShannon ==================

class CoderShannon {
private:
    
    float sum_dict(const std::vector<std::pair<char, float>>& dict);
    
    void Split(const std::vector<std::pair<char, float>>& dict,
        std::vector<std::pair<char, std::string>>& code_result,
        int start_i = 0);
    
    std::string read_file(const std::string& filename);
    
    void write_file(const std::string& filename, const std::string& content);
    
    std::string checkAnswerUser(std::string call, std::vector<std::string> values);
    
public:
    
    void start_encoder();
};


#endif
