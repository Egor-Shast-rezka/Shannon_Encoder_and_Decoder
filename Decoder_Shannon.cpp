/*
    Egor Shastin st129457@student.spbu.ru
    
    Implementation of DecoderShannon class methods for decoding text using the Shannon-Fano algorithm.
*/


#include "Decoder_Shannon.h"


// ================== is_num ==================

bool DecoderShannon::is_num(const std::string elem) { // Check is number or no

    return elem >= "0" && elem <= "9";
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
    
    file.open(filename);
    
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
            output += dictionary[current_num];
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
    std::string input = read_file("data/" + file_name_dictionary);
    std::string input_file = read_file("data/" + file_name);
    
    auto input_correct = utf8_split(input);
    
    map<std::string, std::string> dictionary; // Dictionary for (num : symbol)
    
    std::string last_str = "";
    std::string last_num = "";
    
    // Get data from dictionary
    for (int elem = 0; elem < static_cast<int>(input_correct.size()); ++elem) {

        if (!last_str.empty() && !last_num.empty() && !is_num(input_correct[elem])) {
            
            dictionary.insert(last_num, unescape(last_str));
            last_str = "";
            last_num = "";
        }
        if (last_str.empty() && input_correct[elem] != "\n") {
        
            last_str += input_correct[elem];
            
            if (input_correct[elem] == "\\") {
            
                last_str += input_correct[elem+1];
                elem++;
            }
        }
        else if (!last_str.empty() && is_num(input_correct[elem])) last_num += input_correct[elem];
    }
    if (!last_str.empty() && !last_num.empty()) dictionary.insert(last_num, unescape(last_str));
    
    dictionary.print();
    write_file("data/" + name_output_file, decoder(input_file, dictionary)); // End, Write file
    std::cout << "The output file was recorded successfully!\n";
}
