#include "scanner.h"
#include "common.h"

#include <string_view>

Scanner::Scanner(std::string_view src) 
    : source(src) {
}

Token Scanner::makeToken(TokenType type) {
    return Token{ .type = type,
                  .lexeme = source.substr(start, current - start),
                  .line = line };

}

Token Scanner::errorToken(std::string_view message) {
    return Token{ .type = TOKEN_ERROR,
                  .lexeme = message,
                  .line = line };
}

Token Scanner::stringToken() {

    // The size test is required because advance() is unchecked, so an unterminated string
    // would walk off the end. 
    while (peek()!='"' && current < source.size()) {
        if (peek() == '\n'){
            line++;
        }
        advance();
    }
    if (current == source.size()){
        return errorToken("Unterminated string.");
    }
    advance();
    return makeToken(TOKEN_STRING);
}

Token Scanner::numberToken() {
    while(isDigit(peek())) { // integer part of number
        advance();
    }

    if (peek() == '.' && isDigit(peekNext())) { //Check if decimal point exists
        advance();
    }

    while(isDigit(peek())) { // fractional part of the number
        advance();
    }
    
    return makeToken(TOKEN_NUMBER);
}

Token Scanner::identifierToken() {
    while (isAlpha(peek()) || isDigit(peek())) {
        advance();
    }

    return makeToken(identifierType());
}

char Scanner::advance() { // UNCHECKED. Callers guarantee current < source.size().
    return source[current++];
}

//tests the index directly. Peek returns \0 both for End of String and a real NUL byte.
//this one directly consumes on success.
bool Scanner::match(char expected) {
    if(current == source.size()) {
        return false;
    }
    if(source[current] != expected){
        return false;
    }
    current++;
    return true;
}

char Scanner::peek() const {
    if (current >= source.size()) {
        return '\0'; // not a sentinel, but just automatically fails isDigit/isAlpha. 
    }
    return source[current];
}

//bound on current + 1 because that's what is read. string_view doesn't guarantee null terminator.
char Scanner::peekNext() const {
    if (current + 1 >= source.size()) {
        return '\0';
    }
    return source[current + 1];
}

bool Scanner::isDigit(char c) {
    return (c >= '0' && c <= '9');
}

bool Scanner::isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_');
}

//length check ensures that it's a keyword and not reading 'print' off of 'printer'. 
TokenType Scanner::checkKeyword(size_t offset, size_t length, std::string_view rest, TokenType type) const{
    if (current - start == offset + length && source.substr(start + offset, length) == rest) {
        return type;
    }

    return TOKEN_IDENTIFIER;
}

TokenType Scanner::identifierType() {

    switch (source[start]) {
        case 'a' : return checkKeyword(1, 2, "nd", TOKEN_AND);
        case 'c' : return checkKeyword(1, 4, "lass", TOKEN_CLASS);
        case 'e' : return checkKeyword(1, 3, "lse", TOKEN_ELSE);
        case 'f' : {
            if (current - start > 1) {
                switch (source[start + 1]) {
                    case 'a': return checkKeyword(2, 3, "lse", TOKEN_FALSE);
                    case 'o': return checkKeyword(2, 1, "r", TOKEN_FOR);
                    case 'u': return checkKeyword(2, 1, "n", TOKEN_FUN);
                }
            }
            break;
        }
        case 'i' : return checkKeyword(1, 1, "f", TOKEN_IF);
        case 'n' : return checkKeyword(1, 2, "il", TOKEN_NIL);
        case 'o' : return checkKeyword(1, 1, "r", TOKEN_OR);
        case 'p' : return checkKeyword(1, 4, "rint", TOKEN_PRINT);
        case 'r' : return checkKeyword(1, 5, "eturn", TOKEN_RETURN);
        case 's' : return checkKeyword(1, 4, "uper", TOKEN_SUPER);
        case 't' : {
            if (current - start > 1) {
                switch (source[start + 1]) {
                    case 'h': return checkKeyword(2, 2, "is", TOKEN_THIS);
                    case 'r': return checkKeyword(2, 2, "ue", TOKEN_TRUE);
                }
            }
            break;
        }
        case 'v' : return checkKeyword(1, 2, "ar", TOKEN_VAR);
        case 'w' : return checkKeyword(1, 4, "hile", TOKEN_WHILE);

}

    return TOKEN_IDENTIFIER;
}



void Scanner::skipWhitespace() {
    for (;;) {
        char c = peek();
        switch(c) {
            case ' ':
            case '\r':
            case '\t': {
                advance();
                break;
            }
            case '\n': {
                line++;
                advance();
                break;
            }

            case '/': {
                if (peekNext() == '/') {
                    while (peek() != '\n' && current < source.size()){
                        advance();
                    }
                }
                else{
                    return;
                }
                break;
            }
            default: {
                return;
            }
        }
    }
}

Token Scanner::scanToken() {
    skipWhitespace();
    start = current;

    if(current >= source.size()) {
        return makeToken(TOKEN_EOF);
    }

    char c = advance();

    if (isAlpha(c)) {
        return identifierToken();
    }

    if (isDigit(c)) {
        return numberToken();
    }

    switch (c) {
        case '(' : return makeToken(TOKEN_LEFT_PAREN);
        case ')' : return makeToken(TOKEN_RIGHT_PAREN);
        case '{' : return makeToken(TOKEN_LEFT_BRACE);
        case '}' : return makeToken(TOKEN_RIGHT_BRACE);
        case ';' : return makeToken(TOKEN_SEMICOLON);
        case ',' : return makeToken(TOKEN_COMMA);
        case '.' : return makeToken(TOKEN_DOT);
        case '-' : return makeToken(TOKEN_MINUS);
        case '+' : return makeToken(TOKEN_PLUS);
        case '/' : return makeToken(TOKEN_SLASH);
        case '*' : return makeToken(TOKEN_STAR);
        case '!' : return makeToken(match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
        case '=' : return makeToken(match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
        case '<' : return makeToken(match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
        case '>' : return makeToken(match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
        case '"' : return stringToken();
    }

    return errorToken("Unexpected Character.");
}