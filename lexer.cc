#include "lexer.h"
#include <string_view>
namespace cpu0 {
static bool IsDigit(char ch) {
    return (ch >= '0' && ch <= '9');
}

static bool IsLetter(char ch) {
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch == '_');
}

void Lexer::NextToken(Token &tok) {
    /// 1. 处理空白字符和注释
    SkipWhiteSpaceAndComment();
    
    /// 2. 判断是否到结尾
    if (BufPtr >= BufEnd) {
        tok.kind = TokenType::Eof;
        return;
    }

    tok.row = row;
    tok.col = BufPtr - LineHeadPtr + 1;

    const char *pStart = BufPtr;
    if (BufPtr[0] == '"') {
        BufPtr++;
        tok.kind = TokenType::String;
        tok.ptr = BufPtr;
        std::string value;
        while (*BufPtr != '"') {
            if (!*BufPtr) {
                diagEngine.Report(llvm::SMLoc::getFromPointer(BufPtr), diag::err_unclosed_string);
            }
            value += BufPtr[0];
            BufPtr++;
        }
        tok.strVal = value;
        tok.len = BufPtr - tok.ptr;
        BufPtr++; // skip "
    }else if (IsDigit(*BufPtr)) {
        int num = 0;
        while (IsDigit(*BufPtr)) {
            num = num * 10 + (BufPtr[0] - '0');
            BufPtr++;
        }
        tok.kind = TokenType::Num;
        tok.numVal = num;
        tok.ptr = pStart;
        tok.len = BufPtr - pStart;
    }else if (IsLetter(*BufPtr)) {
        while (IsLetter(*BufPtr) || IsDigit(*BufPtr)) {
            BufPtr++;
        }
        tok.kind = TokenType::Iden;
        tok.ptr = pStart;
        tok.len = BufPtr - pStart;
        std::string_view text = std::string_view(tok.ptr, tok.len);
        if (KeyWordsTable_.find(text) != KeyWordsTable_.end()) {
            tok.kind = KeyWordsTable_[text];
        }
    }else {
        switch (BufPtr[0])
        {
        case ',': {
            tok.kind = TokenType::COMMA;
            tok.ptr = pStart;
            tok.len = 1;
            BufPtr++;
            break;
        }
        case ':': {
            tok.kind = TokenType::COLON;
            tok.ptr = pStart;
            tok.len = 1;
            BufPtr++;
            break;
        }
        default:
            diagEngine.Report(llvm::SMLoc::getFromPointer(BufPtr), diag::err_unknown_char, *BufPtr);
            break;
        }
    }
}

llvm::StringRef Lexer::GetSpellingText(TokenType type) {
    auto v = static_cast<uint8_t>(type);
    if (SpellWordsTable_.find(v) != SpellWordsTable_.end()) {
        return SpellWordsTable_[v];
    }
    llvm::llvm_unreachable_internal();
}

void Lexer::SkipWhiteSpaceAndComment() {
    while (BufPtr[0] == ' ' || BufPtr[0] == '\r' || BufPtr[0] == '\t' || BufPtr[0] == '\n' || BufPtr[0] == ';') {
        if (BufPtr[0] == '\n') {
            row++;
            LineHeadPtr = BufPtr+1;
        }
        /// 处理注释
        if (BufPtr[0] == ';') {
            while (BufPtr[0] != '\n') {
                BufPtr++;
            }
        }else {
            BufPtr++;
        }
    }
}

void Lexer::InitialKeywordTable() {
    KeyWordsTable_ = {
      {"LD", TokenType::LD},
      {"ST", TokenType::ST},
      {"LDB", TokenType::LDB},
      {"STB", TokenType::STB},
      {"LDR", TokenType::LDR},
      {"STR", TokenType::STR},
      {"LBR", TokenType::LBR},
      {"SBR", TokenType::SBR},
      {"LDI", TokenType::LDI},
      {"CMP", TokenType::CMP},
      {"MOV", TokenType::MOV},
      {"ADD", TokenType::ADD},
      {"SUB", TokenType::SUB},
      {"MUL", TokenType::MUL},  
      {"DIV", TokenType::DIV},
      {"AND", TokenType::AND},
      {"OR", TokenType::OR},
      {"XOR", TokenType::XOR},
      {"ADDI", TokenType::ADDI},
      {"ROL", TokenType::ROL},
      {"ROR", TokenType::ROR},
      {"SHL", TokenType::SHL},
      {"SHR", TokenType::SHR},
      {"JEQ", TokenType::JEQ},
      {"JNE", TokenType::JNE},
      {"JLT", TokenType::JLT},
      {"JGT", TokenType::JGT},   
      {"JLE", TokenType::JLE},  
      {"JGE", TokenType::JGE},
      {"JMP", TokenType::JMP},  
      {"SWI", TokenType::SWI},  
      {"CALL", TokenType::CALL},  
      {"RET", TokenType::RET},  
      {"IRET", TokenType::IRET},  
      {"PUSH", TokenType::PUSH},  
      {"POP", TokenType::POP},  
      {"PUSHB", TokenType::PUSHB},  
      {"POPB", TokenType::POPB},  
      {"RESW", TokenType::RESW},  
      {"RESB", TokenType::RESB},  
      {"WORD", TokenType::WORD},  
      {"BYTE", TokenType::BYTE}
    };

    SpellWordsTable_ = {
      {static_cast<uint8_t>(TokenType::LD), "LD"},
      {static_cast<uint8_t>(TokenType::ST), "ST"},
      {static_cast<uint8_t>(TokenType::LDB), "LDB"},
      {static_cast<uint8_t>(TokenType::STB), "STB"},
      {static_cast<uint8_t>(TokenType::LDR), "LDR"},
      {static_cast<uint8_t>(TokenType::STR), "STR"},
      {static_cast<uint8_t>(TokenType::LBR), "LBR"},
      {static_cast<uint8_t>(TokenType::SBR), "SBR"},
      {static_cast<uint8_t>(TokenType::LDI), "LDI"},
      {static_cast<uint8_t>(TokenType::CMP), "CMP"},
      {static_cast<uint8_t>(TokenType::MOV), "MOV"},
      {static_cast<uint8_t>(TokenType::ADD), "ADD"},
      {static_cast<uint8_t>(TokenType::SUB), "SUB"},
      {static_cast<uint8_t>(TokenType::MUL), "MUL"},  
      {static_cast<uint8_t>(TokenType::DIV), "DIV"},
      {static_cast<uint8_t>(TokenType::AND), "AND"},
      {static_cast<uint8_t>(TokenType::OR), "OR"},
      {static_cast<uint8_t>(TokenType::XOR), "XOR"},
      {static_cast<uint8_t>(TokenType::ADDI), "ADDI"},
      {static_cast<uint8_t>(TokenType::ROL), "ROL"},
      {static_cast<uint8_t>(TokenType::ROR), "ROR"},
      {static_cast<uint8_t>(TokenType::SHL), "SHL"},
      {static_cast<uint8_t>(TokenType::SHR), "SHR"},
      {static_cast<uint8_t>(TokenType::JEQ), "JEQ"},
      {static_cast<uint8_t>(TokenType::JNE), "JNE"},
      {static_cast<uint8_t>(TokenType::JLT), "JLT"},
      {static_cast<uint8_t>(TokenType::JGT), "JGT"},  
      {static_cast<uint8_t>(TokenType::JLE), "JLE"},
      {static_cast<uint8_t>(TokenType::JGE), "JGE"},  
      {static_cast<uint8_t>(TokenType::JMP), "JMP"},  
      {static_cast<uint8_t>(TokenType::SWI), "SWI"},  
      {static_cast<uint8_t>(TokenType::CALL), "CALL"},  
      {static_cast<uint8_t>(TokenType::RET), "RET"},  
      {static_cast<uint8_t>(TokenType::IRET), "IRET"},  
      {static_cast<uint8_t>(TokenType::PUSH), "PUSH"},  
      {static_cast<uint8_t>(TokenType::POP), "POP"},  
      {static_cast<uint8_t>(TokenType::PUSHB), "PUSHB"},  
      {static_cast<uint8_t>(TokenType::POPB), "POPB"},  
      {static_cast<uint8_t>(TokenType::RESW), "RESW"},  
      {static_cast<uint8_t>(TokenType::RESB), "RESB"},  
      {static_cast<uint8_t>(TokenType::WORD), "WORD"},  
      {static_cast<uint8_t>(TokenType::BYTE), "BYTE"},                                                                                          
      {static_cast<uint8_t>(TokenType::COLON), ":"},
      {static_cast<uint8_t>(TokenType::COMMA), ","}
    };
}
}