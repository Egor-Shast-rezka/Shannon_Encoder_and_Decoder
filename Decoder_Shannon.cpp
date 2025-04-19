/*
    Egor Shastin st129457@student.spbu.ru
    
    Implementation of DecoderShannon class methods for decoding text using the Shannon-Fano algorithm.
*/


#include "Decoder_Shannon.h"


// ================== is_num ==================

bool DecoderShannon::is_num(char elem) { // Check is number or no

    return elem >= '0' && elem <= '9';
}


// ================== unescape ==================

std::string DecoderShannon::unescape(const std::string& str) { // Check special symbol (\n, \t ...)

    if (str == "\\n") return "\n";
    if (str == "\\t") return "\t";
    if (str == "\\r") return "\r";
    if (str == "\\\\") return "\\";
    if (str == "\\'") return "\'";
    if (str == "\\\"") return "\"";
    
    return str;
}


// ================== read_file ==================

std::string DecoderShannon::read_file(const std::string& filename) { // Read file from txt file

    std::ifstream file(filename);
    
    if (!file.is_open()) std::cerr << "ERROR: File " << filename << " unknown.\n";

    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}


// ================== decoder ==================

std::string DecoderShannon::decoder(std::string input_file, map<std::string, std::string> dictionary) { // Decode text from txt file

    std::string output;
    std::string current_num;
    
    for (char& elem : input_file) {
        
        current_num += elem;
        if (dictionary.find(current_num) != nullptr) {
            
            output += unescape(dictionary[current_num]);
            current_num = "";
        }
    }
    std::cout << "\n";
    return output;
}


// ================== start_decoder ==================

void DecoderShannon::start_decoder() { // Start decoder shannon
 
    // Name for input file and dictionary
    std::string file_name_dictionary = "dictionary.txt";
    std::string name_output_file = "decoder_output.txt";
    std::string file_name = checkAnswerUser("Write name input file (in format txt): ", {".txt"});
    
    // Get data
    std::string str = read_file("data/" + file_name_dictionary);
    std::string input_file = read_file("data/" + file_name);
    
    map<std::string, std::string> dictionary; // Dictionary for (num : symbol)

    std::string last_str = "";
    std::string last_num = "";
    bool in_str = false;
    bool in_num = false;
    
    // Get data from dictionary
    for (int elem = 0; elem < static_cast<int>(str.size()); ++elem) {
        
        // Unique case - sumbol it is '
        if (str[elem] == '\'' && elem + 2 < static_cast<int>(str.size()) && str[elem + 2] == '\'' \
            && str[elem + 1] == '\'' && !in_str && !in_num) {
        
            last_str = "'";
            elem += 2;
        }
        else if ((str[elem] == '\'') && in_str) in_str = !in_str; // Exit line
        else if (str[elem] == '\'') in_str = !in_str; // Enter line
        else if (in_str) last_str += str[elem]; // Add line
        
        if (in_num && !is_num(str[elem])) { // Exit num and make act
        
            in_num = !in_num;
            dictionary[last_num] = last_str;
            last_num = "";
            last_str = "";
        }
        else if (!in_str && !in_num && is_num(str[elem])) { // Enter num
        
            in_num = true;
            last_num += str[elem];
        }
        else if (in_num && is_num(str[elem])) last_num += str[elem]; // Add num
    }
    write_file("data/" + name_output_file, decoder(input_file, dictionary)); // End, Write file
    std::cout << "The output file was recorded successfully!\n";
}
