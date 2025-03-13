/***************************************************************
 *  Student Name: Trevor Mee
 *  File Name: scanner.hpp
 *  Project 1
 * 
 *  @brief This file defines the structures and functions for a 
 *         scanner.
 ***************************************************************/

#ifndef SCANNER_H
#define SCANNER_H

#include <string>
#include <vector>
#include <fstream>
#include <cctype>
#include <unordered_map>
#include <iostream>
#include <set>
#include <variant>
#include <cctype>
#include <algorithm>

// structure to hold token type and token value
struct Token {
    std::string type;
    std::variant<std::monostate, int, std::string> value;
};

class Scanner{

public:
    static const char EOI;
    static const char START_COMMENT;
    static const char END_COMMENT;
    static const char START_STRING;
    static const char END_STRING;
    static const char EQUAL;
    static const char NOT;
    static const char GREATER;
    static const char LESS;

    static const std::set<char> WHITESPACE;
    static const std::set<char> DIGITS;
    static const std::set<char> LETTERS;
    static const std::set<char> LETTERS_OR_DIGITS;

    static const std::unordered_map<char, std::string> OP_TABLE;
    static const std::unordered_map<std::string, std::string> KEYWORD_TABLE;
    static const std::string eoIToken;

    // Source code and scanning state
    std::string source;
    size_t position;           
    std::string currentText;   
    std::string currentToken;
    int lineNumber;

    // function declarations
    Scanner(const std::string& src);
    void init();
    int getLineNumber();
    Token nextToken();
    
private:
    void error(const std::string& msg);
    char currentCh();
    void move();
    bool atEOI();
    void eat();
    std::string find(char x);
    std::string findStar(const std::set<char>& s);
    std::string skip(char x);
    std::string skipStar(const std::set<char>& s);
    void skipWS();
    void skipComment();
    void jump();
    void jumpStar();
    Token NUM();
    Token ID();
    Token STR();
    std::string twoCharSym(char secondCh, const std::string & firstToken, const std::string & secondToken);
    
};
#endif 
