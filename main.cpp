/*
    Egor Shastin st129457@student.spbu.ru
    
    Main file and start all programm.
*/


#include "Encoder_Shannon.h"
#include "Decoder_Shannon.h"


// ================== main ==================

int main(int argc, char* argv[]) {
    
    if (argc == 2) {
        std::string flag = argv[1];
        
        try {
        
            if (flag == "-e") {
            
                CoderShannon coder;
                coder.start_encoder();
            } 
            if (flag == "-d") {
                
                DecoderShannon coder;
                coder.start_decoder();
            }
        } 
        catch (...) {
            std::cerr << "\nUnknown error.\n";
        }
    }
    
    return 0;
}

