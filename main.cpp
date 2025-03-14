/***************************************************************
  Student Name: Trevor Mee
  File Name: main.cpp
  Project 2

  @brief Contains the main function for the scanner, 
         recursive descent and generates RPN code for 
         successfully parsed input files
***************************************************************/

#include "parser.hpp"
#include "scanner.hpp"


int main(int argc, char* argv[]) {
    
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <source_file>" << std::endl;
        return 1;  
    }

    // Open the source file
    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << argv[1] << std::endl;
        return 1;
    }

    // convert input file into a string
    std::string source((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();  

    // Create a Scanner instance 
    Scanner scanner(source);

    // Create a Parser instance
    Parser parser(scanner);

    // Parse the source code
    parser.parse(argv[1]);

    return 0;
}