/*
    Egor Shastin st129457@student.spbu.ru
    
    This is code for testing all classes by using Google Test
*/


#include <gtest/gtest.h>
#include "Decoder_Shannon.h"
#include "Encoder_Shannon.h"
#include "dictionary.h"


#include <cstring>
#include <fstream>
#include <cstdlib>
#include <ctime>


// ================== createRandomTextFile ==================

void createRandomTextFile(const std::string& filename, int length) {

    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "ERROR: Unknown file.\n";
        return;
    }

    const std::string charset = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    for (int i = 0; i < length; ++i) {
        int randomType = std::rand() % 20;

        if (randomType == 0) file << ' ';
        else if (randomType == 1) file << '\n';
        else {
            char randomChar = charset[std::rand() % charset.size()];
            file << randomChar;
        }
    }
    file.close();
}


// ================== compare_files ==================

bool compare_files(const std::string& filename_input, const std::string& filename_output) {

    std::ifstream file1(filename_input, std::ios::binary);
    std::ifstream file2(filename_output, std::ios::binary);

    if (!file1.is_open() || !file2.is_open()) {
        std::cerr << "ERROR: One of the files could not be opened.\n";
        return false;
    }

    const std::size_t buffer_size = 4096;
    char buffer1[buffer_size];
    char buffer2[buffer_size];

    while (true) {
        file1.read(buffer1, buffer_size);
        file2.read(buffer2, buffer_size);

        std::streamsize bytesRead1 = file1.gcount();
        std::streamsize bytesRead2 = file2.gcount();
        
        if (bytesRead1 > 0 && (buffer1[bytesRead1 - 1] == ' ' || buffer1[bytesRead1 - 1] == '\n')) bytesRead1--;
        if (bytesRead2 > 0 && (buffer2[bytesRead2 - 1] == ' ' || buffer2[bytesRead2 - 1] == '\n')) bytesRead2--;
        
        if (bytesRead1 != bytesRead2) return false;
        if (std::memcmp(buffer1, buffer2, bytesRead1) != 0) return false;

        if (file1.eof() && file2.eof()) break;
        if (file1.eof() != file2.eof()) return false;
    }

    return true;
}


// ================== all_test ==================

TEST(EncoderAndDecpderTest, EncoderAndDecpderTest) {

    CoderShannon encoder;
    DecoderShannon decoder;
    
    for (int i = 0; i < 50; ++i) {
        
        // Encoder start and create file
        createRandomTextFile("data/random.txt", 2000);
    
        std::istringstream input_enc("random.txt\n");
        std::streambuf* originalCin_1 = std::cin.rdbuf(input_enc.rdbuf());
        
        encoder.start_encoder();

        std::cin.rdbuf(originalCin_1);
        
        // Decoder start
        std::istringstream input_dec("output.bin\n");
        std::streambuf* originalCin_2 = std::cin.rdbuf(input_dec.rdbuf());

        decoder.start_decoder();

        std::cin.rdbuf(originalCin_2);
        
        // Compare input and output file
        EXPECT_TRUE(compare_files("data/random.txt", "data/decoder_output.txt"));
        if (!compare_files("data/random.txt", "data/decoder_output.txt")) break;
    }
}


// ================== main ==================

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
