//
// Created by jjasmine on 3/8/24.
//

#ifndef SAMMINE_LANG_FILERAII_H
#define SAMMINE_LANG_FILERAII_H
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

class FileRAII {
public:
    // Constructor opens the file
    FileRAII(const std::string& filename) : file(filename), is_opened(true) {
        if (!file.is_open()) {
            std::cerr << "Error: Unable to open file: " << filename << std::endl;
            is_opened = false;
        }
    }

    // Destructor closes the file
    ~FileRAII() {
        if (is_opened) {
            file.close();
        }
    }

    // Function to check if the file is open
    bool isOpen() const {
        return is_opened;
    }

    bool isEOF() const {
        return !file || file.eof();
    }

    // Function to read one character at a time
    char readChar() {
        char ch;
        if (file.get(ch)) {
            return ch;
        } else {
            is_opened = false;  // Mark the file as closed if there are no more characters
            return '\0';       // Return null character to indicate end of file
        }
    }
    // Function to read the entire file into a string
    std::string readFileToString() {
        std::stringstream buffer = {};
        char ch;
        while (file.get(ch)) {
            buffer.put(ch);
        }
        return buffer.str();
    }

private:
    std::ifstream file;
    bool is_opened;
};
#endif //SAMMINE_LANG_FILERAII_H
