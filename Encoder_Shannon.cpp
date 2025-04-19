/*
    Egor Shastin st129457@student.spbu.ru
    
    Header file for the CoderShannon class, which implements the Shannon-Fano algorithm for encoding text.
*/


#include "dictionary.h"
#include "Encoder_Shannon.h"


// ================== main ==================

float CoderShannon::sum_dict(const std::vector<std::pair<char, float>>& dict) { // Sum all second value in dictionarry

    float summa = 0;
    for (const auto& elem : dict) summa += elem.second;
    
    return summa;
}


// ================== Split ==================

void CoderShannon::Split(const std::vector<std::pair<char, float>>& dict, 
    std::vector<std::pair<char, std::string>>& code_result,
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

std::string CoderShannon::read_file(const std::string& filename) { // Read file from txt file

    std::ifstream file(filename);
    std::string exemple_file = "input_example.txt";
    if (!file.is_open()) {
        std::cerr << "Error: file " << filename << " unknown. Use '" << exemple_file << "'.\n";
        std::ifstream file(exemple_file);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}


// ================== write_file ==================

void CoderShannon::write_file(const std::string& filename, const std::string& text) { // Write file in txt file

    std::ofstream file(filename, std::ios::out | std::ios::binary);
    
    if (file.is_open()) {
    
        file.write(text.c_str(), text.size());
        file.close();
        
    } else std::cerr << "Failed to write file: " << filename << "\n";
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


// ================== start_encoder ==================

void CoderShannon::start_encoder() { // Start encoder shannon
    
    // Get user file
    map<char, float> freq;
    std::string file_name = checkAnswerUser("Write name input file (in format txt): ", {".txt"});
    std::string input = read_file("data/" + file_name);
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
    
        write_file("data/" + output_file_name, output_data);
        std::cout << "The output file was recorded successfully!\n";
        
        write_file("data/" + dictionary_output_file_name, dictionary_output_data);
        std::cout << "The dictionary file was recorded successfully!\n";
        
    }
    catch (const std::exception& e) {
    
        std::cerr << "Error: " << e.what() << "\n";
    }
}

