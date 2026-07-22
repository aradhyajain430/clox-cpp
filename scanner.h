#pragma once

#include <string_view>
#include <cstddef>

enum TokenType {
    //Single character tokens
    TOKEN_LEFT_PAREN, TOKEN_RIGHT_PAREN,
    TOKEN_LEFT_BRACE, TOKEN_RIGHT_BRACE,
    TOKEN_COMMA, TOKEN_DOT, TOKEN_MINUS, TOKEN_PLUS,
    TOKEN_SEMICOLON, TOKEN_SLASH, TOKEN_STAR,

    //One or two character tokens
    TOKEN_BANG, TOKEN_BANG_EQUAL,
    TOKEN_EQUAL, TOKEN_EQUAL_EQUAL,
    TOKEN_GREATER, TOKEN_GREATER_EQUAL,
    TOKEN_LESS, TOKEN_LESS_EQUAL,

    //Literals
    TOKEN_IDENTIFIER, TOKEN_STRING, TOKEN_NUMBER,

    //Keywords
    TOKEN_AND, TOKEN_CLASS, TOKEN_ELSE, TOKEN_FALSE,
    TOKEN_FOR, TOKEN_FUN, TOKEN_IF, TOKEN_NIL, TOKEN_OR,
    TOKEN_PRINT, TOKEN_RETURN, TOKEN_SUPER, TOKEN_THIS,
    TOKEN_TRUE, TOKEN_VAR, TOKEN_WHILE,

    TOKEN_ERROR, TOKEN_EOF,

    //Sentinel - must stay last
    TOKEN_COUNT
};

struct Token {
    TokenType type;
    //overloaded - normally a view into source buffer
    //on TOKEN_ERROR it carries the error message literal. 
    std::string_view lexeme;
    int line;
};

class Scanner {
    //NOT OWNED. I REPEAT, NOT OWNED. THIS AND EVERY TOKEN IS A VIEW INTO runFile()'s std::string.
    //THE RUNFILE BUFFER MUST OUTLIVE THIS. Any tokens that escape the scope WILL dangle. 
    //Right now, it only holds because compile and run nest inside runFile.
    std::string_view source;
    size_t current = 0;
    size_t start = 0;
    int line = 1;

    Token makeToken(TokenType type);
    Token errorToken(std::string_view message);
    Token stringToken();
    Token numberToken();
    Token identifierToken();

    char advance();
    bool match(char expected);
    char peek() const;
    char peekNext() const;

    static bool isDigit(char c);
    static bool isAlpha(char c);
    TokenType checkKeyword(size_t offset, size_t length, std::string_view rest, TokenType type) const;
    TokenType identifierType();

    void skipWhitespace();
    

    public:
        Scanner (std::string_view src);
        Token scanToken();
};

