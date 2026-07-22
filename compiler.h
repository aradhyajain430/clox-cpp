#pragma once

#include "scanner.h"
#include "chunk.h"

#include <string_view>

class Compiler {

    enum Precedence {
        PREC_NONE,
        PREC_ASSIGNMENT,   // =
        PREC_OR,           // or
        PREC_AND,          // and
        PREC_EQUALITY,     // == !=
        PREC_COMPARISON,   // < > <= >=
        PREC_TERM,         // + -
        PREC_FACTOR,       // * /
        PREC_UNARY,        // ! -
        PREC_CALL,         // . ()
        PREC_PRIMARY
    };
    using ParseFn = void (Compiler::*)(); // pointer to member because these functions are methods and need a "this" 
    struct ParseRule {
        ParseFn prefix;
        ParseFn infix;
        Precedence precedence;
    };
    static const ParseRule rules[]; // unbounded so that you don't silently zero-fill missing rows.
    static const ParseRule& getRule(TokenType type);

    Scanner scanner;
    Chunk* chunk = nullptr;

    Token previousToken {};
    Token currentToken {};
    bool hadError = false;
    bool panicMode = false; // So only the first error prints. Until there are statements, there's nothing to resync to. 

    void advance();
    void expression();
    void consume(TokenType type, std::string_view message);

    void emitByte(uint8_t byte);
    void emitConstant(Value constant);

    void errorAtCurrent(std::string_view message);
    void error(std::string_view message);
    void errorAt(const Token& token, std::string_view message);

    void number();
    void grouping();
    void unary();
    void binary();

    void parsePrecedence(Precedence precedence);

    public:
    Compiler(std::string_view source);
    bool compile(Chunk& chk);
};