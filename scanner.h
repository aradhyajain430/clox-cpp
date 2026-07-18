#pragma once

#include <string_view>

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
};

struct Token {
    TokenType type;
    std::string_view lexeme;
    int line;
};

class Scanner {
    std::string_view source;
    std::size_t current = 0;
    std::size_t start = 0;
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

    bool isDigit(char c);
    bool isAlpha(char c);
    TokenType checkKeyword(int offset, int length, std::string_view rest, TokenType type) const;
    TokenType identifierType();

    void skipWhitespace();
    

    public:
        Scanner (std::string_view src);
        Token scanToken();
};

