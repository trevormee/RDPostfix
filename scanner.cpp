/***************************************************************
 *  Student Name: Trevor Mee
 *  File Name: scanner.cpp
 *  Project 1
 * 
 *  @brief This file contains the initialization of the scanner class
 *         and the implementation of the scanner functions.
 ***************************************************************/
#include "scanner.hpp"

// Initialize all static members
const char Scanner::EOI = '$';
const char Scanner::START_COMMENT = '~';
const char Scanner::END_COMMENT = '\r';
const char Scanner::START_STRING = '"';
const char Scanner::END_STRING = '"';
const char Scanner::EQUAL = '=';
const char Scanner::NOT = '!';
const char Scanner::GREATER = '>';
const char Scanner::LESS = '<';

const std::set<char> Scanner::WHITESPACE = { ' ', '\t', '\n', '\r'};
const std::set<char> Scanner::DIGITS = { '0','1','2','3','4','5','6','7','8','9' };

// set of letters
const std::set<char> Scanner::LETTERS = {
    'A','B','C','D','E','F','G','H','I','J','K','L','M',
    'N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
    'a','b','c','d','e','f','g','h','i','j','k','l','m',
    'n','o','p','q','r','s','t','u','v','w','x','y','z'
};

// set of letters, digits, and underscore
const std::set<char> Scanner::LETTERS_OR_DIGITS = {
    'A','B','C','D','E','F','G','H','I','J','K','L','M',
    'N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
    'a','b','c','d','e','f','g','h','i','j','k','l','m',
    'n','o','p','q','r','s','t','u','v','w','x','y','z',
    '0','1','2','3','4','5','6','7','8','9', '_', '.'
};

// map of operators to their corresponding token types
const std::unordered_map<char, std::string> Scanner::OP_TABLE = {
    {'(', "lParen"},
    {')', "rParen"},
    {'{', "lCurly"},
    {'}', "rCurly"},
    {'+', "plusSym"},
    {'-', "minusSym"},
    {'*', "timesSym"},
    {'/', "divSym"},
    {';', "semicolon"},
    {',', "comma"}
};

// map of keywords to their corresponding token types
const std::unordered_map<std::string, std::string> Scanner::KEYWORD_TABLE = {
    {"while",  "whileSym"},
    {"return", "returnSym"},
    {"if",     "ifSym"},
    {"else",   "elseSym"},
    {"do",     "doSym"},
    {"int",    "intSym"},
    {"string", "stringSym"},
    {"begin",  "beginSym"},
    {"end.",    "endSym"},
    {"var",     "varSym"}
};

const std::string Scanner::eoIToken = "end.";
 

/*
    @brief parameterized constructor
    @param src the source code to be scanned
*/
    Scanner::Scanner(const std::string& src) : source(src + Scanner::EOI), lineNumber(1) {
        init();
    }

//--------------------------------
// A. Lowest level functions
//--------------------------------

/*
    @brief initializes the scanner state
    @return N/A
*/
    void Scanner::init() {
        position = 0;           
        currentText = "";       
        currentToken = "";      
    }

/*
    @brief prints an error message
    @param message the error message to be printed
    @return N/A
*/
    void Scanner::error(const std::string & message) {
        std::cout << ">>> Error at line " << getLineNumber() << ": " << message << std::endl;
    }


/*
    @brief returns the current character in the source code
    @return the current character
*/
    char Scanner::currentCh() {
        return source[position];
    }

/*
    @brief moves the scanner to the next character in the source code
    @return N/A
*/
    void Scanner::move() {
        if(currentCh() == '\n'){
            lineNumber++;
        }
        position += 1;
    }

/*
    @brief checks if the scanner is at the end of the input
    @return true if the scanner is at the end of the input, false otherwise
*/
    bool Scanner::Scanner::atEOI() {
        return currentCh() == Scanner::EOI;
    }

//--------------------------------   
// B. Second level movements
//--------------------------------

/*
    @brief same as move, but report error for moving past EOI
    @return N/A
*/
    void Scanner::eat() {
        if (atEOI()) {
            error("Cannot move beyond EOI!");
        } else {
            move();
        }
    }

/*
    @brief Find a character "x" down the input stream, 
           Report error if none found before EOICollect 
           all characters found in a result string
    @param x the character to be found
    @return the string of characters found before the character "x"
*/
    std::string Scanner::find(char x) {
        std::string result = "";
        while (currentCh() != x && !atEOI()) {
            result = result + currentCh();
            eat();
        }
        if (atEOI()) {
            error(std::string("EOI detected searching for ") + x);
            return "";
        } else {
            return result;
        }
    }


/*
    @brief Same as find but for a set "s"
    @param s the set of characters to be found
    @return the string of characters found before the character in the set "s"
*/
    std::string Scanner::findStar(const std::set<char>& s) {
        std::string result = "";
        while (s.find(currentCh()) == s.end() && !atEOI()) {
            result = result + currentCh();
            eat();
        }
        if (atEOI()) {
            std::string setString = "";
            for (char ch : s) { setString.push_back(ch); }
            error("EOI detected searching for " + setString);
            return "";
        } else {
            return result;
        }
    }

/* 
    @brief Run over character "x" down the input stream , Report error if 
           none other found before EOI, Collect all characters found in a 
           result string
    @param x the character to be skipped
    @return the string of characters skipped
*/
    std::string Scanner::skip(char x) {
        std::string result = "";
        while (currentCh() == x) {
            result = result + currentCh();
            eat();
        }
        return result;
    }


/*
    @brief Run over characters in set "s" down the input stream, Report error if 
           none other found before EOI, Collect all characters found in a result string
    @param s the set of characters to be skipped
    @return the string of characters skipped
*/
    std::string Scanner::skipStar(const std::set<char>& s) {
        std::string result = "";
        while (s.find(currentCh()) != s.end()) {
            result = result + currentCh();
            eat();
        }
        return result;
    }

/*
    @brief skips over whitespaces
    @return N/A
*/
    void Scanner::skipWS() {
        skipStar(Scanner::WHITESPACE);
    
        while (currentCh() == '\r') { 
            eat();  
            if (currentCh() == '\n') { 
                eat();
            }
            skipStar(Scanner::WHITESPACE);  
        }
    }


  /*
    @brief skips over comments
    @return N/A
  */
    void Scanner::skipComment() {
        while (currentCh() != '\n' && currentCh() != Scanner::EOI) {
            eat();
        }
        if (currentCh() == '\n') {
            eat();
        }
    }
    

/*
    @brief jumps over whitespace and comments
    @return N/A
*/
    void Scanner::jump() {
        if (Scanner::WHITESPACE.find(currentCh()) != Scanner::WHITESPACE.end()) {
            skipWS();
        } else if (currentCh() == Scanner::START_COMMENT) {
            skipComment();
        }
    }

/*
    @brief jumps over whitespace and comments
    @return N/A
*/
    void Scanner::jumpStar() {
        while (Scanner::WHITESPACE.find(currentCh()) != Scanner::WHITESPACE.end() || currentCh() == Scanner::START_COMMENT) {
            jump();
        }
    }


//--------------------------------
// C. High level tokenizers
//--------------------------------

/*
    @brief Skip over digits
    @return the token of type numConstant
*/
    // Token Scanner::NUM() {
    //     std::string numStr = skipStar(Scanner::DIGITS);
    
    //     if (Scanner::LETTERS.find(currentCh()) != Scanner::LETTERS.end()) {
    //         error("Invalid number format: Numbers cannot be followed by letters.");
    //         Token tok;
    //         tok.type = "error";
    //         tok.value = std::monostate{};
    //         return tok;
    //     }
    
    //     Token tok;
    //     tok.type = "numConstant";
    //     tok.value = std::stoi(numStr);
    //     return tok;
    // }
    Token Scanner::NUM() {
        std::string numStr = skipStar(Scanner::DIGITS);
    
        // Ensure numbers are not followed by letters
        if (Scanner::LETTERS.find(currentCh()) != Scanner::LETTERS.end()) {
            error("Invalid number format: Numbers cannot be followed by letters.");
            Token tok;
            tok.type = "error";
            tok.value = std::monostate{};
            return tok;
        }
    
        // Create and return the token
        Token tok;
        tok.type = "numConstant";
        tok.value = std::stoi(numStr); // Ensure value is set as an int
        return tok;
    }
    

/* 
    @brief determines if current token is an identifier
    @return the token of type identifier
*/
    // Token Scanner::ID() {
    //     std::string idStr;

    //     if (Scanner::LETTERS.find(currentCh()) == Scanner::LETTERS.end()) {
    //         error("Identifier must start with a letter.");
    //         Token tok;
    //         tok.type = "error";
    //         tok.value = std::monostate{};
    //         return tok;
    //     }
    
    //     idStr += currentCh();
    //     eat();
        
    //     bool lastWasUnderscore = false;
    
    //     while (Scanner::LETTERS_OR_DIGITS.find(currentCh()) != Scanner::LETTERS_OR_DIGITS.end()) {
    //         if (currentCh() == '_') {
    //             if (lastWasUnderscore) {  
    //                 error("Identifier cannot have consecutive underscores.");
    //                 Token tok;
    //                 tok.type = "error";
    //                 tok.value = std::monostate{};
    //                 return tok;
    //             }
    //             lastWasUnderscore = true;
    //         } else {
    //             lastWasUnderscore = false;
    //         }
    
    //         idStr += currentCh();
    //         eat();
    //     }
    
    //     if (idStr.back() == '_') {
    //         error("Identifier cannot end with an underscore.");
    //         Token tok;
    //         tok.type = "error";
    //         tok.value = std::monostate{};
    //         return tok;
    //     }
    
    //     Token tok;
    //     tok.type = "identifier";
    //     tok.value = idStr;
    //     return tok;
    // }

    Token Scanner::ID() {
        std::string idStr;
    
        // Ensure the identifier starts with a letter
        if (Scanner::LETTERS.find(currentCh()) == Scanner::LETTERS.end()) {
            error("Identifier must start with a letter.");
            Token tok;
            tok.type = "error";
            tok.value = std::monostate{};
            return tok;
        }
    
        // Add the first character to the identifier string
        idStr += currentCh();
        eat();
    
        // Process the remaining characters
        bool lastWasUnderscore = false;
        while (Scanner::LETTERS_OR_DIGITS.find(currentCh()) != Scanner::LETTERS_OR_DIGITS.end()) {
            if (currentCh() == '_') {
                if (lastWasUnderscore) {
                    error("Identifier cannot have consecutive underscores.");
                    Token tok;
                    tok.type = "error";
                    tok.value = std::monostate{};
                    return tok;
                }
                lastWasUnderscore = true;
            } else {
                lastWasUnderscore = false;
            }
    
            idStr += currentCh();
            eat();
        }
    
        // Ensure the identifier does not end with an underscore
        if (idStr.back() == '_') {
            error("Identifier cannot end with an underscore.");
            Token tok;
            tok.type = "error";
            tok.value = std::monostate{};
            return tok;
        }
    
        // Debug: Print the identifier string
        std::cout << "DEBUG: Scanned word = " << idStr << std::endl;
    
        // Check if the scanned word is a keyword
        if (KEYWORD_TABLE.find(idStr) != KEYWORD_TABLE.end()) {
            // Return a keyword token
            Token tok;
            tok.type = KEYWORD_TABLE.at(idStr); // Set the token type from the keyword table
            tok.value = std::monostate{};      // Keywords have no value
            std::cout << "DEBUG: Keyword detected: " << idStr << " -> " << tok.type << std::endl;
            return tok;
        } else {
            // Return an identifier token
            Token tok;
            tok.type = "identifier";
            tok.value = idStr; // Set the value to the identifier string
            std::cout << "DEBUG: Identifier detected: " << idStr << std::endl;
            return tok;
        }
    }
/*
    @brief Skip over everything up to end of string
    @return string scanned
*/
    Token Scanner::STR() {
        eat();
        std::string chars = find(Scanner::END_STRING);
        eat();
        Token tok;
        tok.type = "stringConstant";
        tok.value = chars;
        return tok;
    }


/*  
    @breif Scan a possibly two-character token, like == or >=
    @param(s) secondCh the second character to be checked
              firstToken the first token to be returned if secondCh is not found
              secondToken the second token to be returned if secondCh is found
    @return the token of type firstToken or secondToken

*/
    std::string Scanner::twoCharSym(char secondCh, const std::string & firstToken, const std::string & secondToken) {
        eat();
        if (currentCh() == secondCh) {
            eat();
            return secondToken;
        } else {
            return firstToken;
        }
    }

/*
    @brief Main tokenizer
    @return the class and lexeme of the next token
*/
    Token Scanner::nextToken() {
        
        // Trivial test of EOI (End Of Input)
        if (atEOI()) {
            Token tok;
            tok.type = Scanner::eoIToken;
            tok.value = std::monostate{};
            return tok; 
        }

        // find token start
        jumpStar();

        // get current character
        char c = currentCh();

        // All the possibilities (hopefully)

        // Arbitrary long tokens (numbers, strings, ids/reserved words)
        if (Scanner::DIGITS.find(c) != Scanner::DIGITS.end()) {
            return NUM();
        }
        if (c == Scanner::START_STRING) {
            return STR();
        }
        // tmee
        // if (Scanner::LETTERS.find(c) != Scanner::LETTERS.end()) {
        //     Token idToken = ID();
        //     std::string idStr = std::get<std::string>(idToken.value);
        //     if (Scanner::KEYWORD_TABLE.find(idStr) != Scanner::KEYWORD_TABLE.end()) {
        //         Token tok;
        //         tok.type = Scanner::KEYWORD_TABLE.at(idStr);
        //         tok.value = std::monostate{};
        //         return tok;      
        //     } else {
        //         return idToken;  
        //     }
        // }
        if (Scanner::LETTERS.find(c) != Scanner::LETTERS.end()) {
            return ID(); // Call ID() to handle identifiers and keywords
        }

        // Two-char tokens: ==, !=, >=, <=
        if (c == Scanner::EQUAL) {
            Token tok;
            tok.type = twoCharSym(Scanner::EQUAL, "assignSym", "equalSym");
            tok.value = std::monostate{};
            return tok;
        }
        if (c == Scanner::NOT) {
            Token tok;
            tok.type = twoCharSym(Scanner::EQUAL, "notSym", "notEqualSym");
            tok.value = std::monostate{};
            return tok;
        }
        if (c == Scanner::GREATER) {
            Token tok;
            tok.type = twoCharSym(Scanner::EQUAL, "greaterSym", "greaterEQSym");
            tok.value = std::monostate{};
            return tok;
        }
        if (c == Scanner::LESS) {
            Token tok;
            tok.type = twoCharSym(Scanner::EQUAL, "lessSym", "lessEQSym");
            tok.value = std::monostate{};
            return tok;
        }

        // One-char tokens
        if (Scanner::OP_TABLE.find(c) != Scanner::OP_TABLE.end()) {
            eat();
            Token tok;
            tok.type = Scanner::OP_TABLE.at(c);
            tok.value = std::monostate{};
            return tok;
        }

        // Shrug it off, no idea!
        Token tok;
        tok.type = "";
        tok.value = std::monostate{};

        return tok;
    }

    /*
        @brief gets the current line number
        @return current line number
    */
    int Scanner::getLineNumber(){
        return lineNumber;
    }