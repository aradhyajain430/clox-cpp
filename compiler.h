#pragma once

#include "scanner.h"
#include "vm.h"

#include <random>
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
    using ParseFn = void (Compiler::*)();
    struct ParseRule {
        ParseFn prefix;
        ParseFn infix;
        Precedence precedence;
    };
    static const ParseRule rules[];
    static const ParseRule& getRule(TokenType type);

    Scanner scanner;
    Chunk* chunk = nullptr;

    Token previousToken {};
    Token currentToken {};
    bool hadError = false;
    bool panicMode = false;

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