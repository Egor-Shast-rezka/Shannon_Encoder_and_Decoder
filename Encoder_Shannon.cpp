/*
    Egor Shastin st129457@student.spbu.ru
    
    Header file for the CoderShannon class, which implements the Shannon-Fano algorithm for encoding text.
*/


#include "dictionary.h"
#include "Encoder_Shannon.h"


// ================== sum_dict ==================

float CoderShannon::sum_dict(const std::vector<std::pair<std::string, float>>& dict) { // Sum all second value in dictionarry

    float summa = 0;
    for (const auto& elem : dict) summa += elem.second;
    
    return summa;
}


// ================== Split ==================

void CoderShannon::Split(const std::vector<std::pair<std::string, float>>& dict, 
    std::vector<std::pair<std::string, std::string>>& code_result,
    int start_i) { // Get each symbol unique num

    if (dict.empty()) return;

    if (dict.size() == 1) {
        code_result[start_i].second += "0";
        return;
    }

    if (dict.size() == 2) {
        code_result[start_i].second += "0";
        code_result[start_i + 1].second += "1";
        return;
    }

    std::vector<std::pair<std::string, float>> first_pare;
    std::vector<std::pair<std::string, float>> second_pare;

    float sum = 0;
    float sum_all_dict = sum_dict(dict) * 0.5;
    int i = 0;

    while (i < static_cast<int>(dict.size()) && sum + dict[i].second <= sum_all_dict) {
    
        sum += dict[i].second;
        first_pare.push_back(dict[i]);
        code_result[start_i + i].second += "0";
        i++;
    }

    Split(first_pare, code_result, start_i);

    while (i < static_cast<int>(dict.size())) {
    
        second_pare.push_back(dict[i]);
        code_result[start_i + i].second += "1";
        i++;
    }

    Split(second_pare, code_result, start_i + (int)first_pare.size());
}


// ================== read_file ==================

std::string CoderShannon::read_file(const std::string& filename) { // Read file from txt file

    std::ifstream file(filename);
    std::string exemple_file = "input_example.txt";
    
    if (!file.is_open()) {
    
        std::cerr << "Error: file " << filename << " unknown. Use '" << exemple_file << "'.\n";
        file.open(exemple_file);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}


// ================== write_file ==================

void CoderShannon::write_file(const std::string& filename, const std::string& content) {

    std::ofstream file(filename);
    if (!file.is_open()) throw std::runtime_error("Couldn't open the file: " + filename);

    file << content;
}


// ================== checkAnswerUser ==================

std::string CoderShannon::checkAnswerUser(std::string call, std::vector<std::string> values) { // Check user answer
    
    std::cout << call;
    
    std::string AnswerUser;
    std::getline(std::cin, AnswerUser);
    
    while (AnswerUser.empty()) {
        
        std::cout << "Invalid input. Please try again.\n";
        std::cin.clear();
        
        std::cout << call;
        std::getline(std::cin, AnswerUser);
    }
    
    if (AnswerUser.size() < 4 || AnswerUser.substr(AnswerUser.size() - 4) != ".txt") {
    
        AnswerUser += ".txt";
    }
    
    return AnswerUser;
}


// ================== escape_char ==================

std::string CoderShannon::escape_char(const std::string& ch) { // Check special symbol (\n, \t ...)

    if (ch == "\n") return "\\n";
    if (ch == "\t") return "\\t";
    if (ch == "\r") return "\\r";
    if (ch == "\\") return "\\\\";
    if (ch == "\'") return "\\\'";
    if (ch == "\"") return "\\\"";

    return ch;
}


// ================== utf8_split ==================

// The function splits a UTF-8 encoded string into individual characters.
std::vector<std::string> CoderShannon::utf8_split(const std::string& str) { 

    std::vector<std::string> result;
    
    for (size_t i = 0; i < str.size();) {
    
        unsigned char c = str[i];
        size_t len = 1;
        
        // 1, 2, 3 and 4-byte
        if ((c & 0x80) == 0x00) len = 1;
        else if ((c & 0xE0) == 0xC0) len = 2;
        else if ((c & 0xF0) == 0xE0) len = 3;
        else if ((c & 0xF8) == 0xF0) len = 4;

        result.push_back(str.substr(i, len));
        i += len;
    }
    return result;
}


// ================== start_encoder ==================

void CoderShannon::start_encoder() { // Start encoder shannon
    
    // Name for output file
    std::string output_file_name = "output.txt";
    std::string dictionary_output_file_name = "dictionary.txt";
    
    // Get user file
    std::string file_name = checkAnswerUser("Write name input file (in format txt): ", {".txt"});
    std::string input = read_file("data/" + file_name);
    auto symbols = utf8_split(input);
    
    map<std::string, float> freq;
    for (const std::string& ch : symbols) freq[ch] += 1;
    
    // Create sorted dictionary
    auto begin = freq.begin();
    auto end = freq.end();
    
    std::vector<std::pair<std::string, float>> sorted_dict(begin, end);

    std::sort(sorted_dict.begin(), sorted_dict.end(),
        [](const std::pair<std::string, float>& a, const std::pair<std::string, float>& b) {
            return a.second > b.second;
        });
    
    // Add chanse for all symbol in dictionary
    for (auto& pair : sorted_dict) pair.second /= freq.size();
    
    // Make shannon code 
    std::vector<std::pair<std::string, std::string>> shannon_code;
    for (const auto& pair : sorted_dict) shannon_code.push_back({pair.first, ""});

    Split(sorted_dict, shannon_code);
    
    map<std::string, std::string> shannon_code_dict;
    for (auto& pair : shannon_code) shannon_code_dict.insert(pair.first, pair.second);
    
    // Create output file
    std::string output_data;
    std::string dictionary_output_data;
    
    for (auto& ch : symbols) output_data += shannon_code_dict[ch];
    for (auto& pair : shannon_code) {
        dictionary_output_data += escape_char(pair.first) + ": " + pair.second + "\n";
    }
    
    // Writing in output file
    try {
    
        write_file("data/" + output_file_name, output_data);
        std::cout << "The output file was recorded successfully!\n";
        
        write_file("data/" + dictionary_output_file_name, dictionary_output_data);
        std::cout << "The dictionary file was recorded successfully!\n";
        
    }
    catch (const std::exception& e) {
    
        std::cerr << "Error: " << e.what() << "\n";
    }
    
}
