#include "chunk.h"
#include "common.h"
#include "scanner.h"
#include "compiler.h"
#include "debug.h"


#include <iostream>
#include <format>
#include <string_view>
#include <charconv>
#include <iterator>



Compiler::Compiler(std::string_view src) 
    : scanner(src) {
}

//ROW ORDER MUST MATCH TOKEN ORDER EXACTLY. 
//Trailing // comments are documentation.
//DO NOT delete unused tokens. DO NOT delete stuff randomly.
//ONLY ADD STUFF IN PLACE. MAINTAIN THE ORDER SPECIFIED IN scanner.h.  
const Compiler::ParseRule Compiler::rules[] = {
    {&Compiler::grouping, nullptr, PREC_NONE},   // TOKEN_LEFT_PAREN
    {nullptr, nullptr, PREC_NONE},   // TOKEN_RIGHT_PAREN
    {nullptr, nullptr, PREC_NONE},   // TOKEN_LEFT_BRACE
    {nullptr, nullptr, PREC_NONE},   // TOKEN_RIGHT_BRACE
    {nullptr, nullptr, PREC_NONE},   // TOKEN_COMMA
    {nullptr, nullptr, PREC_NONE},   // TOKEN_DOT
    {&Compiler::unary, &Compiler::binary, PREC_TERM},   // TOKEN_MINUS
    {nullptr, &Compiler::binary, PREC_TERM},   // TOKEN_PLUS
    {nullptr, nullptr, PREC_NONE},   // TOKEN_SEMICOLON
    {nullptr, &Compiler::binary, PREC_FACTOR},   // TOKEN_SLASH
    {nullptr, &Compiler::binary, PREC_FACTOR},   // TOKEN_STAR
    {nullptr, nullptr, PREC_NONE},   // TOKEN_BANG
    {nullptr, nullptr, PREC_NONE},   // TOKEN_BANG_EQUAL
    {nullptr, nullptr, PREC_NONE},   // TOKEN_EQUAL
    {nullptr, nullptr, PREC_NONE},   // TOKEN_EQUAL_EQUAL
    {nullptr, nullptr, PREC_NONE},   // TOKEN_GREATER
    {nullptr, nullptr, PREC_NONE},   // TOKEN_GREATER_EQUAL
    {nullptr, nullptr, PREC_NONE},   // TOKEN_LESS
    {nullptr, nullptr, PREC_NONE},   // TOKEN_LESS_EQUAL
    {nullptr, nullptr, PREC_NONE},   // TOKEN_IDENTIFIER
    {nullptr, nullptr, PREC_NONE},   // TOKEN_STRING
    {&Compiler::number, nullptr, PREC_NONE},   // TOKEN_NUMBER
    {nullptr, nullptr, PREC_NONE},   // TOKEN_AND
    {nullptr, nullptr, PREC_NONE},   // TOKEN_CLASS
    {nullptr, nullptr, PREC_NONE},   // TOKEN_ELSE
    {nullptr, nullptr, PREC_NONE},   // TOKEN_FALSE
    {nullptr, nullptr, PREC_NONE},   // TOKEN_FOR
    {nullptr, nullptr, PREC_NONE},   // TOKEN_FUN
    {nullptr, nullptr, PREC_NONE},   // TOKEN_IF
    {nullptr, nullptr, PREC_NONE},   // TOKEN_NIL
    {nullptr, nullptr, PREC_NONE},   // TOKEN_OR
    {nullptr, nullptr, PREC_NONE},   // TOKEN_PRINT
    {nullptr, nullptr, PREC_NONE},   // TOKEN_RETURN
    {nullptr, nullptr, PREC_NONE},   // TOKEN_SUPER
    {nullptr, nullptr, PREC_NONE},   // TOKEN_THIS
    {nullptr, nullptr, PREC_NONE},   // TOKEN_TRUE
    {nullptr, nullptr, PREC_NONE},   // TOKEN_VAR
    {nullptr, nullptr, PREC_NONE},   // TOKEN_WHILE
    {nullptr, nullptr, PREC_NONE},   // TOKEN_ERROR
    {nullptr, nullptr, PREC_NONE},   // TOKEN_EOF
};

const Compiler::ParseRule& Compiler::getRule(TokenType type) {
    static_assert(std::size(rules) == TOKEN_COUNT, "Rule table and TokenType enum are out of sync.");
    return rules[type];
} 

bool Compiler::compile(Chunk& chk) {
    
    chunk = &chk;

    advance();
    expression();
    consume(TOKEN_EOF, "Expect end of expression.");

    emitByte(OP_RETURN);
    if constexpr (DEBUG_PRINT_CODE) {
        if (!hadError) {
            disassembleChunk(*chunk, "code");
        }
    }

    return !hadError;
}

void Compiler::advance() {
    previousToken = currentToken;

    for (;;) {
        currentToken = scanner.scanToken();
        if (currentToken.type != TOKEN_ERROR) {
            break;
        }

        errorAtCurrent(currentToken.lexeme);
    }
}

void Compiler::expression() {
    parsePrecedence(PREC_ASSIGNMENT);
}

void Compiler::consume(TokenType type, std::string_view message) {
    if (currentToken.type == type) {
        advance();
        return;
    }

    errorAtCurrent(message);
}

//previousToken - you emit the byte of the token you have just consumed. 
void Compiler::emitByte(uint8_t byte) {
    chunk->write(byte, previousToken.line);
}

void Compiler::emitConstant(Value constant) {
    chunk->writeConstant(constant, previousToken.line);
}


void Compiler::errorAtCurrent(std::string_view message) {
    errorAt(currentToken, message);
}

void Compiler::error(std::string_view message) {
    errorAt(previousToken, message);
}

void Compiler::errorAt(const Token& token, std::string_view message) {
    if (panicMode) {
        return;
    }
    panicMode = true;

    std::cerr << std::format("[line {}] Error", token.line);
    if (token.type == TOKEN_EOF) {
        std::cerr << " at end";
    }
    else if (token.type == TOKEN_ERROR) {
        //nothing - the token.lexeme holds the error message, which shouldn't get pasted twice.
    }
    else {
        std::cerr << std::format(" at '{}'", token.lexeme);
    }
    std::cerr << std::format(": {}\n", message);

    hadError = true;
}

void Compiler::number() {
    double value = 0;
    const char* begin = previousToken.lexeme.data();
    const char* end = begin + previousToken.lexeme.size();

    //This part is to validate the magnitude of the number - you can't store a 400 digit number. 
    auto [ptr, ec] = std::from_chars(begin, end, value);
    if (ec != std::errc{}) {
        error("Number literal out of range.");
        return;
    }
    emitConstant(value);
}

void Compiler::grouping() {
    expression();
    consume(TOKEN_RIGHT_PAREN, "Expect ')' after expression.");
}

void Compiler::unary() {
    TokenType opType = previousToken.type;

    parsePrecedence(PREC_UNARY);

    switch(opType) {
        case TOKEN_MINUS: emitByte(OP_NEGATE); break;
        default: return;
    }
}

void Compiler::binary() {
    TokenType opType = previousToken.type;
    const ParseRule& rule = getRule(opType);
    // the + 1 makes it left associative - it ignores an equal precedence operator. 
    parsePrecedence(static_cast<Precedence>(rule.precedence + 1));

    switch (opType) {
        case TOKEN_PLUS:  emitByte(OP_ADD); break;
        case TOKEN_MINUS: emitByte(OP_SUBTRACT); break;
        case TOKEN_STAR:  emitByte(OP_MULTIPLY); break;
        case TOKEN_SLASH: emitByte(OP_DIVIDE); break;
        default: return; //unreachable
    }
}

void Compiler::parsePrecedence(Precedence precedence) {
    advance();
    ParseFn prefixRule = getRule(previousToken.type).prefix;
    if (prefixRule == nullptr) {
        error("Expect expression.");
        return;
    }

    (this->*prefixRule)();

    while (precedence <= getRule(currentToken.type).precedence) {
        advance();
        ParseFn infixRule = getRule(previousToken.type).infix;
        //Unchecked, safe only by table invariant - every row 
        //with precedence > PREC_NONE has an infix rule. 
        (this->*infixRule)(); 
    }
}