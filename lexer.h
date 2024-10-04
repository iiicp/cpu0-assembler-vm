#pragma once
#include <string>
#include <unordered_map>
#include "llvm/ADT/StringMap.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/raw_ostream.h"
#include "diag_engine.h"
#include "token.h"
namespace cpu0 {
class Lexer {
private:
    llvm::SourceMgr &mgr;
    DiagEngine &diagEngine;
    llvm::StringRef fileName;
public:
    Lexer(llvm::SourceMgr &mgr, DiagEngine &diagEngine): mgr(mgr), diagEngine(diagEngine) { 
        unsigned id = mgr.getMainFileID();
        llvm::StringRef buf = mgr.getMemoryBuffer(id)->getBuffer();
        BufPtr = buf.begin();
        LineHeadPtr = buf.begin();
        BufEnd = buf.end();
        row = 1;
        fileName = mgr.getMemoryBuffer(id)->getBufferIdentifier();
        InitialKeywordTable();
    };
    void NextToken(Token &tok);
    llvm::StringRef GetSpellingText(TokenType type);
private:
    void SkipWhiteSpaceAndComment();
    void InitialKeywordTable();
private:
    const char *BufPtr;
    const char *LineHeadPtr;
    const char *BufEnd;
    int row;
    llvm::StringMap<TokenType> KeyWordsTable_;
    std::unordered_map<std::underlying_type_t<TokenType>, llvm::StringRef> SpellWordsTable_;
};
}