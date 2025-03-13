/***************************************************************
  Student Name: Trevor Mee
  File Name: parser.cpp
  Project 1

  @brief Contains the member function definitions for parsing
         an input source.
***************************************************************/

#include "parser.hpp"

/*
    @brief Parameterized constructor
    @param scanner Scanner object to be used for parsing
    @return N/A
*/
Parser::Parser(Scanner& scanner) : scanner(scanner), lastLabel(-1), IR() {
    lookahead = scanner.nextToken();
}


/*
    @brief parses the input source code
    @return N/A
*/
void Parser::parse() 
{
    try{
        program();
        if (lookahead.type != "endSym") {
            error("Expected end. but found " + lookahead.type);
        }
        std::cout << "Successfully parsed!" << std::endl;
        printRPN();
    }catch (const std::exception& e){
        std::cerr << "parsing error: " << e.what() << std::endl;
    }
}


/*
    @brief prints an error message
    @param message the error message to be printed
    @return N/A
*/
void Parser::error(const std::string& message)
{
    std::cout << ">>> Error line " << scanner.getLineNumber() << ": " << message << std::endl;
    exit(1);
}


/*
    @brief helper function to format error messages
    @param expectedToken the expected token
    @return N/A
*/
void Parser::formatError(const std::string& expectedToken)
{
    std::stringstream errorMsg;
    errorMsg << "Expected '" << expectedToken << "' but found '" << lookahead.type << "' with lexeme '";
    if (std::holds_alternative<int>(lookahead.value)) {
        errorMsg << std::get<int>(lookahead.value);
    } else if (std::holds_alternative<std::string>(lookahead.value)) {
        errorMsg << std::get<std::string>(lookahead.value);
    } else {
        errorMsg << "none";
     }
    errorMsg << "'";
    error(errorMsg.str());
}


/*
    @brief helper function to check if the current token matches the expected token
    @param expectedToken the expected token
    @return N/A
*/
void Parser::expect(const std::string& expectedToken)
{
    if (lookahead.type == expectedToken) {
        if (expectedToken != "endSym") {
            lookahead = scanner.nextToken(); 
        }
    } else {
        formatError(expectedToken);
    }
}


/*
    @brief defines what a program should look like
    @return N/A

    Syntax: "begin" varDeclarations Statements "end."
*/
void Parser::program()
{
    expect("beginSym");
    VarDeclarations();
    while(lookahead.type != "endSym"){
        Stmts();
    }
    expect("endSym");
}


/*
    @brief defines what an assignment should look like
    @return N/A

    Syntax: Identifier "=" Expr ;
*/
void Parser::assignment()
{
    std::string id = Identifier();
    expect("assignSym"); 
    expression(); 
    emit("STORE", id); 
}


/*
    @brief defines what an expression should look like
    @return N/A

    Syntax: Term { ("+" | "-") Term } ;
*/
void Parser::expression()
{
    term();
    while(lookahead.type == "plusSym" || lookahead.type == "minusSym")
    {
        std::string op = lookahead.type;
        scan();
        term();
        emit(opMap[op], "");
    }
}


/*
    @brief defines what an assignment should look like
    @return N/A

    Syntax: factor { ("*" | "/") factor } ;
*/
void Parser::term()
{
    factor();
    while (lookahead.type == "timesSym" || lookahead.type == "divSym")
    {
        std::string op = lookahead.type;
        scan();
        factor();
        emit(opMap[op], "");
    }
}


/*
    @brief defines what an assignment should look like
    @return N/A

    Syntax: identifier | numConstant | "(" Expr ")" ;
*/
void Parser::factor()
{
    if (lookahead.type == "identifier") {
        
        if (!std::holds_alternative<std::string>(lookahead.value)) {
            error("Expected identifier value to be a string");
        }
        std::string id = std::get<std::string>(lookahead.value);
        
        if (symbolTable.find(id) == symbolTable.end()) {
            error("Undefined variable " + id);
        }
        
        emit("EVAL", id);
        scan();
        } 
        else if (lookahead.type == "numConstant") {
            
            if (!std::holds_alternative<int>(lookahead.value)) {
            error("Expected numeric value to be an integer");
            }

            int numValue = std::get<int>(lookahead.value);
        
            emit("PUSH", std::to_string(numValue));
            scan();
        } 
        else if (lookahead.type == "lParen") {
            scan();
            expression();
            expect("rParen");
        } 
        else {
            error("Expected identifier, number, or left parenthesis");
        }
}


/*
    @brief prints the identifiers used in a valid input file
    @return N/A
*/
void Parser::printIdentifiers()
{
    std::cout << "Identifiers used in the program:" << std::endl;
    for(const auto& id: symbolTable) {
        std::cout << id << std::endl;
    }
}


/*
    @brief Create distinct symbolic labels L0, L1, etc
    @return the new label
*/
std::string Parser::newLabel()
{
    lastLabel++;
    return "L" + std::to_string(lastLabel);
}


/*
    @brief Emits one RPN operation into RPN stream
    @param(s)  tag: token type 
               item: token value 
    @return N/A
*/
void Parser::emit(const std::string& tag, const std::string& item = "")
{
    IR.push_back({tag, item});
}


/*
    @brief Interfaces with the scanner
    @return N/A
*/
void Parser::scan()
{
    lookahead = scanner.nextToken();
}

/*
    @brief Parses statements
    @return N/A

    Syntax: Stmt { ";" Stmt } ;
*/
void Parser::Stmts()
{
    Stmt();
    while(lookahead.type == "semicolon")
    {
        expect("semicolon");
        if(lookahead.type != "endSym"){
            Stmt();
        }
    }
}


/*
    @brief Parses one individual statement
    @return N/A

    Syntax: Assign | Cond | Loop | "end."
*/
void Parser::Stmt()
{
    if(lookahead.type == "identifier"){
        assignment();
    }
    else if(lookahead.type == "ifSym"){
        Cond();
    }
    else if(lookahead.type == "whileSym"){
        Loop();
    }else if(lookahead.type == "endSym"){
        // end of program
    }
    else{
        error("id, if or while expected");
    }
}


/*
    @brief Parses conditions
    @return N/A

    Syntax: "if" "(" Expr ")" Stmt;
*/
void Parser::Cond()
{
    std::string skipLabel = newLabel();
    scan();
    expect("lParen");
    expression();
    expect("rParen");
    emit("BZ", skipLabel);
    Stmt();
    emit("LABEL", skipLabel);
}

/*
    @brief Parses loops
    @return N/A

    Syntax: "while" "(" Expr ")" Stmt ;
*/
void Parser::Loop()
{
    std::string repeatLabel = newLabel();
    std::string skiplabel = newLabel();
    scan();
    emit("LABEL", repeatLabel);
    expect("lParen");
    expression();
    expect("rParen");
    emit("BZ", skiplabel);
    Stmt();
    emit("BR", repeatLabel);
    emit("LABEL", skiplabel);
}

/*
    @brief semantic trick that identifies an identifier
    @return returns name of identifier if true, empty string if false

    Syntax: "var" { LETTERS_DIGITS } 
*/
std::string Parser::Identifier()
{
    if (lookahead.type == "identifier") {
        if (!std::holds_alternative<std::string>(lookahead.value)) {
            error("Expected identifier value to be a string");
        }
        std::string id = std::get<std::string>(lookahead.value);
        scan();
        return id;
    }
    error("Identifier expected");
    return "";
}

/*
    @brief Writes the generated RPN to an output file
    @return N/A
*/
void Parser::printRPN()
{
    std::cout << "Printing RPN..." << std::endl;
    for(const auto& x : IR)
    {
        if(x.second.empty()){
            std::cout << "['" << x.first << "']" << std::endl;
        }else{
            std::cout << "['" << x.first << ", '" << x.second << "']" << std::endl;
        }
    }
}


/*
    @brief parses and handles variable declarations
    @return N/A

    Syntax: "var" IdentifierList ";" { "var" IdentifierList ";" }

    Note: IdentifierList refers to the symbolTable holding identifiers
*/
void Parser::VarDeclarations()
{
    while (lookahead.type == "varSym") {
        expect("varSym"); 

        do {
            std::string varName = Identifier(); 
            if (symbolTable.find(varName) != symbolTable.end()) {
                error("Illegal redefinition of variable " + varName);
            } else {
                symbolTable.insert(varName); 
            }

            if (lookahead.type == "comma") {
                expect("comma");
            }
        } while (lookahead.type == "identifier");

        expect("semicolon");
    }
}
