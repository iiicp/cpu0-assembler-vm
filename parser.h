#pragma once
#include "lexer.h"
#include "inst.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/StringMap.h"
#include "llvm/ADT/SmallVector.h"

namespace cpu0 {
class Parser {
private:
    Lexer &lexer;
    DiagEngine &diagEngine;
public:
    Parser(Lexer &lexer, DiagEngine &diagEngine) : lexer(lexer), diagEngine(diagEngine) {
        InitialRegMap();
        Advance();
    }
    const llvm::SmallVector<std::shared_ptr<Inst>> &Parse();
    const llvm::DenseMap<llvm::StringRef, std::shared_ptr<Inst>> &SymbolTable() {
        return symbolTable;
    }
private:
    std::shared_ptr<Inst> ParseInst();
    std::shared_ptr<Inst> ParseLD();
    std::shared_ptr<Inst> ParseST();
    std::shared_ptr<Inst> ParseLDB();
    std::shared_ptr<Inst> ParseSTB();
    std::shared_ptr<Inst> ParseLDR();
    std::shared_ptr<Inst> ParseSTR();
    std::shared_ptr<Inst> ParseLBR();
    std::shared_ptr<Inst> ParseSBR();
    std::shared_ptr<Inst> ParseLDI();
    std::shared_ptr<Inst> ParseCMP();
    std::shared_ptr<Inst> ParseMOV();
    std::shared_ptr<Inst> ParseADD();
    std::shared_ptr<Inst> ParseSUB();
    std::shared_ptr<Inst> ParseMUL();
    std::shared_ptr<Inst> ParseDIV();
    std::shared_ptr<Inst> ParseAND();
    std::shared_ptr<Inst> ParseOR();
    std::shared_ptr<Inst> ParseXOR();
    std::shared_ptr<Inst> ParseADDI();
    std::shared_ptr<Inst> ParseROL();
    std::shared_ptr<Inst> ParseROR();
    std::shared_ptr<Inst> ParseSHL();
    std::shared_ptr<Inst> ParseSHR();
    std::shared_ptr<Inst> ParseJEQ();
    std::shared_ptr<Inst> ParseJNE();
    std::shared_ptr<Inst> ParseJLT();
    std::shared_ptr<Inst> ParseJGT();
    std::shared_ptr<Inst> ParseJLE();
    std::shared_ptr<Inst> ParseJGE();
    std::shared_ptr<Inst> ParseJMP();
    std::shared_ptr<Inst> ParseSWI();
    std::shared_ptr<Inst> ParseCALL();
    std::shared_ptr<Inst> ParseRET();
    std::shared_ptr<Inst> ParseIRET();
    std::shared_ptr<Inst> ParsePUSH();
    std::shared_ptr<Inst> ParsePOP();
    std::shared_ptr<Inst> ParsePUSHB();
    std::shared_ptr<Inst> ParsePOPB();
    std::shared_ptr<Inst> ParseRESW();
    std::shared_ptr<Inst> ParseRESB();
    std::shared_ptr<Inst> ParseWORD();
    std::shared_ptr<Inst> ParseBYTE();
private:
    /// 消费 token 的函数
    /// 检测当前 token是否是该类型，不会消费
    bool Expect(TokenType tokenType);
    /// 检测，并消费
    bool Consume(TokenType tokenType);
    /// 前进一个 token
    void Advance();

    bool IsInstName();

    void InitialRegMap();

    Token tok;
    llvm::SmallVector<std::shared_ptr<Inst>> instVec;
    llvm::DenseMap<llvm::StringRef, std::shared_ptr<Inst>> symbolTable;
    llvm::StringMap<int> regMap;
};
}