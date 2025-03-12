/***************************************************************
  Student Name: Trevor Mee
  File Name: main.cpp
  Project 1

  @brief Contains the member function declarations for parsing
         an input source.
***************************************************************/

#include "scanner.hpp"
#include <unordered_set>
#include <sstream>
#include <vector>
#include <optional>

#ifndef PARSER_H
#define PARSER_H

class Parser 
{
public:
    // public function declarations
    Parser(Scanner& scanner);
    void parse();

private:
    // private member variables
    Scanner& scanner;
    Token lookahead;
    std::unordered_set<std::string> symbolTable;

    // new variables
    int lastLabel;
    std::vector<std::pair<std::string, std::string>> IR;
    std::unordered_map<std::string, std::string> opMap {
        {"plusSym", "PLUS"},
        {"minusSym", "MINUS"},
        {"timesSym", "TIMES"},
        {"divSym", "DIV"}
    };


    // private function declarations
    void error(const std::string& message);
    void formatError(const std::string& expectedToken);
    void expect(const std::string& expectedToken);
    void program();
    void assignment();
    void expression();
    void term();
    void factor();
    void printIdentifiers();

    // new functions
    std::string newLabel();
    void emit(const std::string& tag, const std::string& item );
    void scan();
    void Stmts();
    void Stmt();
    void Cond();
    void Loop();
    std::string Identifier();
    void printRPN();
    void VarDeclarations();
};
#endif