/*
    Egor Shastin st129457@student.spbu.ru
    
    
*/


#include "dictionary.h"
#include "Encoder_Shannon.h"


// ================== main ==================

float CoderShannon::sum_dict(const std::vector<std::pair<char, float>>& dict) {
    float summa = 0;
    for (const auto& elem : dict) summa += elem.second;
    return summa;
}


// ================== Split ==================

void CoderShannon::Split(const std::vector<std::pair<char, float>>& dict, 
    std::vector<std::pair<char, std::string>>& code_result,
    int start_i) {

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

    std::vector<std::pair<char, float>> first_pare;
    std::vector<std::pair<char, float>> second_pare;

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

std::string CoderShannon::read_file(const std::string& filename) {

    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Unknown file: " + filename);
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

std::string CoderShannon::checkAnswerUser(std::string call, std::vector<std::string> values) {
    
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


// ================== start_encoder ==================

void CoderShannon::start_encoder() {
    
    // Get user file
    map<char, float> freq;
    std::string file_name = checkAnswerUser("Write name input file (in format txt): ", {".txt"});
    std::string input = read_file(file_name);
    for (char ch : input) freq[ch] += 1;
    
    auto begin = freq.begin();
    auto end = freq.end();
    
    std::vector<std::pair<char, float>> sorted_dict(begin, end);

    std::sort(sorted_dict.begin(), sorted_dict.end(),
        [](const std::pair<char, float>& a, const std::pair<char, float>& b) {
            return a.second > b.second;
        });

    for (auto& pair : sorted_dict) pair.second /= freq.size();

    std::vector<std::pair<char, std::string>> shannon_code;
    for (const auto& pair : sorted_dict)
        shannon_code.push_back({pair.first, ""});

    Split(sorted_dict, shannon_code);

    map<char, std::string> shannon_code_dict;
    for (const auto& pair : shannon_code) shannon_code_dict[pair.first] = pair.second;
    
    // Create output file
    std::string output_data;
    std::string dictionary_output_data;
    for (char ch : input) output_data += shannon_code_dict[ch];
    for (const auto& pair : shannon_code) {
        if (std::string(1, pair.first) == "\n") {
        
            dictionary_output_data += "'\\n' : " + pair.second + "\n";
            
        } else dictionary_output_data += "'" + std::string(1, pair.first) + "' : " + pair.second + "\n";
    }
    
    // Name for output file
    std::string output_file_name = "output.txt";
    std::string dictionary_output_file_name = "dictionary.txt";
    
    // Writing in output file
    try {
    
        write_file(output_file_name, output_data);
        std::cout << "The file was recorded successfully!\n";
        
        write_file(dictionary_output_file_name, dictionary_output_data);
        std::cout << "The file was recorded successfully!\n";
        
    }
    catch (const std::exception& e) {
    
        std::cerr << "Error: " << e.what() << "\n";
    }
}

