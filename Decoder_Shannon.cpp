/*
    Egor Shastin st129457@student.spbu.ru
    
    Implementation of DecoderShannon class methods for decoding text using the Shannon-Fano algorithm.
*/


#include "Decoder_Shannon.h"


// ================== is_num ==================

bool DecoderShannon::is_num(const std::string elem) const { // Check is number or no

    return elem >= "0" && elem <= "9";
}


// ================== unescape ==================

std::string DecoderShannon::unescape(const std::string& str) const { // Check special symbol (\n, \t ...)

    if (str == "\\n") return "\n";
    if (str == "\\t") return "\t";
    if (str == "\\r") return "\r";
    if (str == "\\\\") return "\\";
    if (str == "\\'") return "\'";
    if (str == "\\\"") return "\"";
    
    return str;
}


// ================== decoder ==================

std::string DecoderShannon::decoder(const std::string input_file, map<std::string, std::string>& dictionary) { // Decode text from txt file

    std::string output;
    std::string current_num;
    
    for (const char& elem : input_file) {
        
        current_num += elem;
        
        if (dictionary.find(current_num) != nullptr) {
        
            output += dictionary[current_num];
            current_num = "";
        }
    }
    std::cout << "\n";
    return output;
}


// ================== insertInDictionary ==================

void DecoderShannon::insertInDictionary(const std::vector<std::string>& input_correct, map<std::string, std::string>& dictionary) {

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
}


// ================== start_decoder ==================

void DecoderShannon::start_decoder() { // Start decoder shannon
    
    // Name file and data for all code
    // -------------------------
    const std::string file_name_dictionary = "dictionary.txt";
    const std::string name_output_file = "decoder_output.txt";
    const size_t block_size = 4096;
    // -------------------------
    
    // Get user file
    std::string file_name = checkAnswerUser("Write name input file (in format txt): ", {".bin"});
    
    // Get data from dictionary
    std::vector<char> buffer(block_size);
    map<std::string, std::string> dictionary; // Dictionary for (num : symbol)
    
    std::ifstream file_input_dictionary("data/" + file_name_dictionary, std::ios::binary);
    
    if (!file_input_dictionary.is_open()) throw std::runtime_error("Unknown file.");
    
    while (file_input_dictionary) {
        
        file_input_dictionary.read(buffer.data(), block_size);
        std::streamsize bytes_read = file_input_dictionary.gcount();
        
        if (bytes_read > 0) {

            std::string str(buffer.data(), bytes_read);
            insertInDictionary(utf8_split(str), dictionary);
        }
    }
    file_input_dictionary.close();
    //dictionary.print();
    
    // Get data from main input file
    std::ifstream file_input("data/" + file_name, std::ios::binary);
    std::ofstream output_data("data/" + name_output_file, std::ios::binary);
    
    if (!file_input.is_open()) throw std::runtime_error("Unknown file.");
    
    char ch;
    std::string current_code;
    
    while (file_input.get(ch)) {
        
        std::bitset<8> bits(static_cast<unsigned char>(ch));
        current_code += bits.to_string();
        
        while (!current_code.empty()) {
        
            bool found = false;
            
            for (const auto& [code, symbol] : dictionary) {
                if (current_code.rfind(code, 0) == 0) {
                
                    output_data << symbol;
                    current_code = current_code.substr(code.size());
                    found = true;
                    break;
                }
            }
            if (!found) break;
        }
    }
    file_input.close();
    output_data.close();
    
    std::cout << "The output file was recorded successfully!\n";
}
