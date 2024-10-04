#include "parser.h"
namespace cpu0 {
const llvm::SmallVector<std::shared_ptr<Inst>> & Parser::Parse() {
    int32_t addr = 0;
    while (tok.kind != TokenType::Eof) {
        std::shared_ptr<Inst> inst = nullptr;
        if (IsInstName()) {
            inst = ParseInst();
            inst->SetAddr(addr);
            instVec.push_back(inst);
        }else {
            Expect(TokenType::Iden);
            llvm::StringRef symbol(tok.ptr, tok.len);
            Consume(TokenType::Iden);
            Consume(TokenType::COLON);
            inst = ParseInst();
            inst->SetAddr(addr);
            inst->SetLable(symbol);
            instVec.push_back(inst);
            symbolTable.insert({symbol, inst});
        }
        addr += inst->Size();
    }
    return instVec;
}

std::shared_ptr<Inst> Parser::ParseInst() {
    switch (tok.kind)
    {
    case TokenType::LD:
        return ParseLD();
    case TokenType::ST:
        return ParseST();
    case TokenType::LDB:
        return ParseLDB();
    case TokenType::STB:
        return ParseSTB();
    case TokenType::LDR:
        return ParseLDR();
    case TokenType::STR:  
        return ParseSTR();  
    case TokenType::LBR:
        return ParseLBR();  
    case TokenType::SBR:
        return ParseSBR();  
    case TokenType::LDI:
        return ParseLDI();  
    case TokenType::CMP:
        return ParseCMP();  
    case TokenType::MOV:
        return ParseMOV();
    case TokenType::ADD:
        return ParseADD();
    case TokenType::SUB:
        return ParseSUB();
    case TokenType::MUL:  
        return ParseMUL();  
    case TokenType::DIV:
        return ParseDIV();  
    case TokenType::AND:
        return ParseAND();  
    case TokenType::OR:
        return ParseOR();  
    case TokenType::XOR:
        return ParseXOR();  
    case TokenType::ADDI:
        return ParseADDI();  
    case TokenType::ROL:
        return ParseROL();  
    case TokenType::ROR:
        return ParseROR();  
    case TokenType::SHL:    
        return ParseSHL();  
    case TokenType::SHR:
        return ParseSHR();  
    case TokenType::JEQ:
        return ParseJEQ();  
    case TokenType::JNE:
        return ParseJNE();  
    case TokenType::JLT:
        return ParseJLT();  
    case TokenType::JGT:
        return ParseJGT();  
    case TokenType::JLE:
        return ParseJLE();  
    case TokenType::JGE:
        return ParseJGE();  
    case TokenType::JMP:    
        return ParseJMP();  
    case TokenType::SWI:
        return ParseSWI();  
    case TokenType::CALL:
        return ParseCALL();  
    case TokenType::RET:
        return ParseRET();  
    case TokenType::IRET:
        return ParseIRET();  
    case TokenType::PUSH:
        return ParsePUSH();
    case TokenType::POP:
        return ParsePOP();
    case TokenType::PUSHB:
        return ParsePUSHB();
    case TokenType::POPB:
        return ParsePOPB();
    case TokenType::RESW:  
        return ParseRESW();  
    case TokenType::RESB:
        return ParseRESB();  
    case TokenType::WORD:
        return ParseWORD();  
    case TokenType::BYTE:
        return ParseBYTE();  
    default:
        llvm::llvm_unreachable_internal();
        break;
    }
}

std::shared_ptr<Inst> Parser::ParseLD() {
    Consume(TokenType::LD);
    Expect(TokenType::Iden);
    llvm::StringRef r1(tok.ptr, tok.len);
    if (regMap.find(r1) == regMap.end()) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "register");
    }
    int reg1 = regMap[r1];
    Consume(TokenType::Iden);
    Consume(TokenType::COMMA);
    
    Expect(TokenType::Iden);
    llvm::StringRef r2(tok.ptr, tok.len);
    int reg2 = 15;
    if (regMap.find(r2) != regMap.end()) {
        reg2 = regMap[r2];
        Consume(TokenType::Iden);
        Consume(TokenType::COMMA);        
    }

    if (tok.kind != TokenType::Iden && tok.kind != TokenType::Num) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "symbol or number");
    }
    std::variant<llvm::StringRef, int> cx;
    if (tok.kind == TokenType::Iden) {
        cx = llvm::StringRef(tok.ptr, tok.len);
    }else {
        cx = tok.numVal;
    }
    Advance();

    auto t = std::make_shared<LD>();
    t->reg1 = reg1;
    t->reg2 = reg2;
    t->cx = cx;
    return t;
}

std::shared_ptr<Inst> Parser::ParseST() {
    Consume(TokenType::ST);
    Expect(TokenType::Iden);
    llvm::StringRef r1(tok.ptr, tok.len);
    if (regMap.find(r1) == regMap.end()) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "register");
    }
    int reg1 = regMap[r1];
    Consume(TokenType::Iden);
    Consume(TokenType::COMMA);
    
    Expect(TokenType::Iden);
    llvm::StringRef r2(tok.ptr, tok.len);
    int reg2 = 15;
    if (regMap.find(r2) != regMap.end()) {
        reg2 = regMap[r2];
        Consume(TokenType::Iden);
        Consume(TokenType::COMMA);        
    }

    if (tok.kind != TokenType::Iden && tok.kind != TokenType::Num) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "symbol or number");
    }
    std::variant<llvm::StringRef, int> cx;
    if (tok.kind == TokenType::Iden) {
        cx = llvm::StringRef(tok.ptr, tok.len);
    }else {
        cx = tok.numVal;
    }
    Advance();

    auto t = std::make_shared<ST>();
    t->reg1 = reg1;
    t->reg2 = reg2;
    t->cx = cx;
    return t;
}

std::shared_ptr<Inst> Parser::ParseLDB() {
    Consume(TokenType::LDB);
    Expect(TokenType::Iden);
    llvm::StringRef r1(tok.ptr, tok.len);
    if (regMap.find(r1) == regMap.end()) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "register");
    }
    int reg1 = regMap[r1];
    Consume(TokenType::Iden);
    Consume(TokenType::COMMA);
    
    Expect(TokenType::Iden);
    llvm::StringRef r2(tok.ptr, tok.len);
    int reg2 = 15;
    if (regMap.find(r2) != regMap.end()) {
        reg2 = regMap[r2];
        Consume(TokenType::Iden);
        Consume(TokenType::COMMA);        
    }

    if (tok.kind != TokenType::Iden && tok.kind != TokenType::Num) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "symbol or number");
    }
    std::variant<llvm::StringRef, int> cx;
    if (tok.kind == TokenType::Iden) {
        cx = llvm::StringRef(tok.ptr, tok.len);
    }else {
        cx = tok.numVal;
    }
    Advance();

    auto t = std::make_shared<LDB>();
    t->reg1 = reg1;
    t->reg2 = reg2;
    t->cx = cx;
    return t;
}

std::shared_ptr<Inst> Parser::ParseSTB() {
    Consume(TokenType::STB);
    Expect(TokenType::Iden);
    llvm::StringRef r1(tok.ptr, tok.len);
    if (regMap.find(r1) == regMap.end()) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "register");
    }
    int reg1 = regMap[r1];
    Consume(TokenType::Iden);
    Consume(TokenType::COMMA);
    
    Expect(TokenType::Iden);
    llvm::StringRef r2(tok.ptr, tok.len);
    int reg2 = 15;
    if (regMap.find(r2) != regMap.end()) {
        reg2 = regMap[r2];
        Consume(TokenType::Iden);
        Consume(TokenType::COMMA);        
    };

    if (tok.kind != TokenType::Iden && tok.kind != TokenType::Num) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "symbol or number");
    }
    std::variant<llvm::StringRef, int> cx;
    if (tok.kind == TokenType::Iden) {
        cx = llvm::StringRef(tok.ptr, tok.len);
    }else {
        cx = tok.numVal;
    }
    Advance();

    auto t = std::make_shared<STB>();
    t->reg1 = reg1;
    t->reg2 = reg2;
    t->cx = cx;
    return t;
}

std::shared_ptr<Inst> Parser::ParseLDR() {
    Consume(TokenType::LDR);
    Expect(TokenType::Iden);
    llvm::StringRef r1(tok.ptr, tok.len);
    if (regMap.find(r1) == regMap.end()) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "register");
    }
    int reg1 = regMap[r1];
    Consume(TokenType::Iden);
    Consume(TokenType::COMMA);
    
    Expect(TokenType::Iden);
    llvm::StringRef r2(tok.ptr, tok.len);
    if (regMap.find(r2) == regMap.end()) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "register");
    }
    int reg2 = regMap[r2];
    Consume(TokenType::Iden);
    Consume(TokenType::COMMA);

    Expect(TokenType::Iden);
    llvm::StringRef r3(tok.ptr, tok.len);
    if (regMap.find(r3) == regMap.end()) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "register");
    }
    int reg3 = regMap[r3];
    Consume(TokenType::Iden);

    auto t = std::make_shared<LDR>();
    t->reg1 = reg1;
    t->reg2 = reg2;
    t->reg3 = reg3;
    return t;
}

std::shared_ptr<Inst> Parser::ParseSTR() {
    Consume(TokenType::STR);
    Expect(TokenType::Iden);
    llvm::StringRef r1(tok.ptr, tok.len);
    if (regMap.find(r1) == regMap.end()) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "register");
    }
    int reg1 = regMap[r1];
    Consume(TokenType::Iden);
    Consume(TokenType::COMMA);
    
    Expect(TokenType::Iden);
    llvm::StringRef r2(tok.ptr, tok.len);
    if (regMap.find(r2) == regMap.end()) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "register");
    }
    int reg2 = regMap[r2];
    Consume(TokenType::Iden);
    Consume(TokenType::COMMA);

    Expect(TokenType::Iden);
    llvm::StringRef r3(tok.ptr, tok.len);
    if (regMap.find(r3) == regMap.end()) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "register");
    }
    int reg3 = regMap[r3];
    Consume(TokenType::Iden);

    auto t = std::make_shared<STR>();
    t->reg1 = reg1;
    t->reg2 = reg2;
    t->reg3 = reg3;
    return t;
}

std::shared_ptr<Inst> Parser::ParseLBR() {
    Consume(TokenType::LBR);
    Expect(TokenType::Iden);
    llvm::StringRef r1(tok.ptr, tok.len);
    if (regMap.find(r1) == regMap.end()) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "register");
    }
    int reg1 = regMap[r1];
    Consume(TokenType::Iden);
    Consume(TokenType::COMMA);
    
    Expect(TokenType::Iden);
    llvm::StringRef r2(tok.ptr, tok.len);
    if (regMap.find(r2) == regMap.end()) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "register");
    }
    int reg2 = regMap[r2];
    Consume(TokenType::Iden);
    Consume(TokenType::COMMA);

    Expect(TokenType::Iden);
    llvm::StringRef r3(tok.ptr, tok.len);
    if (regMap.find(r3) == regMap.end()) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "register");
    }
    int reg3 = regMap[r3];
    Consume(TokenType::Iden);

    auto t = std::make_shared<LBR>();
    t->reg1 = reg1;
    t->reg2 = reg2;
    t->reg3 = reg3;
    return t;
}

std::shared_ptr<Inst> Parser::ParseSBR() {
    Consume(TokenType::SBR);
    Expect(TokenType::Iden);
    llvm::StringRef r1(tok.ptr, tok.len);
    if (regMap.find(r1) == regMap.end()) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "register");
    }
    int reg1 = regMap[r1];
    Consume(TokenType::Iden);
    Consume(TokenType::COMMA);
    
    Expect(TokenType::Iden);
    llvm::StringRef r2(tok.ptr, tok.len);
    if (regMap.find(r2) == regMap.end()) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "register");
    }
    int reg2 = regMap[r2];
    Consume(TokenType::Iden);
    Consume(TokenType::COMMA);

    Expect(TokenType::Iden);
    llvm::StringRef r3(tok.ptr, tok.len);
    if (regMap.find(r3) == regMap.end()) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "register");
    }
    int reg3 = regMap[r3];
    Consume(TokenType::Iden);

    auto t = std::make_shared<SBR>();
    t->reg1 = reg1;
    t->reg2 = reg2;
    t->reg3 = reg3;
    return t;
}

std::shared_ptr<Inst> Parser::ParseLDI() {
    Consume(TokenType::LDI);
    Expect(TokenType::Iden);
    llvm::StringRef r1(tok.ptr, tok.len);
    if (regMap.find(r1) == regMap.end()) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "register");
    }
    int reg1 = regMap[r1];
    Consume(TokenType::Iden);
    Consume(TokenType::COMMA);
    
    Expect(TokenType::Num);
    int num = tok.numVal;
    Consume(TokenType::Num);

    auto t = std::make_shared<LDI>();
    t->reg1 = reg1;
    t->num = num;
    return t;
}

std::shared_ptr<Inst> Parser::ParseCMP() {
    Consume(TokenType::CMP);
    Expect(TokenType::Iden);
    llvm::StringRef r1(tok.ptr, tok.len);
    if (regMap.find(r1) == regMap.end()) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "register");
    }
    int reg1 = regMap[r1];
    Consume(TokenType::Iden);
    Consume(TokenType::COMMA);
    
    Expect(TokenType::Iden);
    llvm::StringRef r2(tok.ptr, tok.len);
    if (regMap.find(r2) == regMap.end()) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "register");
    }
    int reg2 = regMap[r2];
    Consume(TokenType::Iden);

    auto t = std::make_shared<CMP>();
    t->reg1 = reg1;
    t->reg2 = reg2;
    return t;
}

std::shared_ptr<Inst> Parser::ParseMOV() {
    Consume(TokenType::MOV);
    Expect(TokenType::Iden);
    llvm::StringRef r1(tok.ptr, tok.len);
    if (regMap.find(r1) == regMap.end()) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "register");
    }
    int reg1 = regMap[r1];
    Consume(TokenType::Iden);
    Consume(TokenType::COMMA);
    
    Expect(TokenType::Iden);
    llvm::StringRef r2(tok.ptr, tok.len);
    if (regMap.find(r2) == regMap.end()) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "register");
    }
    int reg2 = regMap[r2];
    Consume(TokenType::Iden);

    auto t = std::make_shared<MOV>();
    t->reg1 = reg1;
    t->reg2 = reg2;
    return t;
}

std::shared_ptr<Inst> Parser::ParseADD() {
    Consume(TokenType::ADD);
    Expect(TokenType::Iden);
    llvm::StringRef r1(tok.ptr, tok.len);
    if (regMap.find(r1) == regMap.end()) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "register");
    }
    int reg1 = regMap[r1];
    Consume(TokenType::Iden);
    Consume(TokenType::COMMA);
    
    Expect(TokenType::Iden);
    llvm::StringRef r2(tok.ptr, tok.len);
    if (regMap.find(r2) == regMap.end()) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "register");
    }
    int reg2 = regMap[r2];
    Consume(TokenType::Iden);
    Consume(TokenType::COMMA);

    Expect(TokenType::Iden);
    llvm::StringRef r3(tok.ptr, tok.len);
    if (regMap.find(r3) == regMap.end()) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "register");
    }
    int reg3 = regMap[r3];
    Consume(TokenType::Iden);

    auto t = std::make_shared<ADD>();
    t->reg1 = reg1;
    t->reg2 = reg2;
    t->reg3 = reg3;
    return t;
}

std::shared_ptr<Inst> Parser::ParseSUB() {
    Consume(TokenType::SUB);
    Expect(TokenType::Iden);
    llvm::StringRef r1(tok.ptr, tok.len);
    if (regMap.find(r1) == regMap.end()) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "register");
    }
    int reg1 = regMap[r1];
    Consume(TokenType::Iden);
    Consume(TokenType::COMMA);
    
    Expect(TokenType::Iden);
    llvm::StringRef r2(tok.ptr, tok.len);
    if (regMap.find(r2) == regMap.end()) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "register");
    }
    int reg2 = regMap[r2];
    Consume(TokenType::Iden);
    Consume(TokenType::COMMA);

    Expect(TokenType::Iden);
    llvm::StringRef r3(tok.ptr, tok.len);
    if (regMap.find(r3) == regMap.end()) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "register");
    }
    int reg3 = regMap[r3];
    Consume(TokenType::Iden);

    auto t = std::make_shared<SUB>();
    t->reg1 = reg1;
    t->reg2 = reg2;
    t->reg3 = reg3;
    return t;
}

std::shared_ptr<Inst> Parser::ParseMUL() {
    Consume(TokenType::MUL);
    Expect(TokenType::Iden);
    llvm::StringRef r1(tok.ptr, tok.len);
    if (regMap.find(r1) == regMap.end()) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "register");
    }
    int reg1 = regMap[r1];
    Consume(TokenType::Iden);
    Consume(TokenType::COMMA);
    
    Expect(TokenType::Iden);
    llvm::StringRef r2(tok.ptr, tok.len);
    if (regMap.find(r2) == regMap.end()) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "register");
    }
    int reg2 = regMap[r2];
    Consume(TokenType::Iden);
    Consume(TokenType::COMMA);

    Expect(TokenType::Iden);
    llvm::StringRef r3(tok.ptr, tok.len);
    if (regMap.find(r3) == regMap.end()) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "register");
    }
    int reg3 = regMap[r3];
    Consume(TokenType::Iden);

    auto t = std::make_shared<MUL>();
    t->reg1 = reg1;
    t->reg2 = reg2;
    t->reg3 = reg3;
    return t;
}

std::shared_ptr<Inst> Parser::ParseDIV() {
    Consume(TokenType::DIV);
    Expect(TokenType::Iden);
    llvm::StringRef r1(tok.ptr, tok.len);
    if (regMap.find(r1) == regMap.end()) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "register");
    }
    int reg1 = regMap[r1];
    Consume(TokenType::Iden);
    Consume(TokenType::COMMA);
    
    Expect(TokenType::Iden);
    llvm::StringRef r2(tok.ptr, tok.len);
    if (regMap.find(r2) == regMap.end()) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "register");
    }
    int reg2 = regMap[r2];
    Consume(TokenType::Iden);
    Consume(TokenType::COMMA);

    Expect(TokenType::Iden);
    llvm::StringRef r3(tok.ptr, tok.len);
    if (regMap.find(r3) == regMap.end()) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "register");
    }
    int reg3 = regMap[r3];
    Consume(TokenType::Iden);

    auto t = std::make_shared<DIV>();
    t->reg1 = reg1;
    t->reg2 = reg2;
    t->reg3 = reg3;
    return t;
}

std::shared_ptr<Inst> Parser::ParseAND() {
    Consume(TokenType::AND);
    Expect(TokenType::Iden);
    llvm::StringRef r1(tok.ptr, tok.len);
    if (regMap.find(r1) == regMap.end()) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "register");
    }
    int reg1 = regMap[r1];
    Consume(TokenType::Iden);
    Consume(TokenType::COMMA);
    
    Expect(TokenType::Iden);
    llvm::StringRef r2(tok.ptr, tok.len);
    if (regMap.find(r2) == regMap.end()) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "register");
    }
    int reg2 = regMap[r2];
    Consume(TokenType::Iden);
    Consume(TokenType::COMMA);

    Expect(TokenType::Iden);
    llvm::StringRef r3(tok.ptr, tok.len);
    if (regMap.find(r3) == regMap.end()) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "register");
    }
    int reg3 = regMap[r3];
    Consume(TokenType::Iden);

    auto t = std::make_shared<AND>();
    t->reg1 = reg1;
    t->reg2 = reg2;
    t->reg3 = reg3;
    return t;
}

std::shared_ptr<Inst> Parser::ParseOR() {
    Consume(TokenType::OR);
    Expect(TokenType::Iden);
    llvm::StringRef r1(tok.ptr, tok.len);
    if (regMap.find(r1) == regMap.end()) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "register");
    }
    int reg1 = regMap[r1];
    Consume(TokenType::Iden);
    Consume(TokenType::COMMA);
    
    Expect(TokenType::Iden);
    llvm::StringRef r2(tok.ptr, tok.len);
    if (regMap.find(r2) == regMap.end()) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "register");
    }
    int reg2 = regMap[r2];
    Consume(TokenType::Iden);
    Consume(TokenType::COMMA);

    Expect(TokenType::Iden);
    llvm::StringRef r3(tok.ptr, tok.len);
    if (regMap.find(r3) == regMap.end()) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "register");
    }
    int reg3 = regMap[r3];
    Consume(TokenType::Iden);

    auto t = std::make_shared<OR>();
    t->reg1 = reg1;
    t->reg2 = reg2;
    t->reg3 = reg3;
    return t;
}

std::shared_ptr<Inst> Parser::ParseXOR() {
    Consume(TokenType::XOR);
    Expect(TokenType::Iden);
    llvm::StringRef r1(tok.ptr, tok.len);
    if (regMap.find(r1) == regMap.end()) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "register");
    }
    int reg1 = regMap[r1];
    Consume(TokenType::Iden);
    Consume(TokenType::COMMA);
    
    Expect(TokenType::Iden);
    llvm::StringRef r2(tok.ptr, tok.len);
    if (regMap.find(r2) == regMap.end()) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "register");
    }
    int reg2 = regMap[r2];
    Consume(TokenType::Iden);
    Consume(TokenType::COMMA);

    Expect(TokenType::Iden);
    llvm::StringRef r3(tok.ptr, tok.len);
    if (regMap.find(r3) == regMap.end()) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "register");
    }
    int reg3 = regMap[r3];
    Consume(TokenType::Iden);

    auto t = std::make_shared<XOR>();
    t->reg1 = reg1;
    t->reg2 = reg2;
    t->reg3 = reg3;
    return t;
}

std::shared_ptr<Inst> Parser::ParseADDI() {
    Consume(TokenType::ADDI);
    Expect(TokenType::Iden);
    llvm::StringRef r1(tok.ptr, tok.len);
    if (regMap.find(r1) == regMap.end()) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "register");
    }
    int reg1 = regMap[r1];
    Consume(TokenType::Iden);
    Consume(TokenType::COMMA);
    
    Expect(TokenType::Iden);
    llvm::StringRef r2(tok.ptr, tok.len);
    if (regMap.find(r2) == regMap.end()) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "register");
    }
    int reg2 = regMap[r2];
    Consume(TokenType::Iden);
    Consume(TokenType::COMMA);

    Expect(TokenType::Num);
    int cx = tok.numVal;
    Advance();

    auto t = std::make_shared<ADDI>();
    t->reg1 = reg1;
    t->reg2 = reg2;
    t->cx = cx;
    return t;
}

std::shared_ptr<Inst> Parser::ParseROL() {
    Consume(TokenType::ROL);
    Expect(TokenType::Iden);
    llvm::StringRef r1(tok.ptr, tok.len);
    if (regMap.find(r1) == regMap.end()) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "register");
    }
    int reg1 = regMap[r1];
    Consume(TokenType::Iden);
    Consume(TokenType::COMMA);
    
    Expect(TokenType::Iden);
    llvm::StringRef r2(tok.ptr, tok.len);
    if (regMap.find(r2) == regMap.end()) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "register");
    }
    int reg2 = regMap[r2];
    Consume(TokenType::Iden);
    Consume(TokenType::COMMA);

    Expect(TokenType::Num);
    int cx = tok.numVal;
    Advance();

    auto t = std::make_shared<ROL>();
    t->reg1 = reg1;
    t->reg2 = reg2;
    t->cx = cx;
    return t;
}

std::shared_ptr<Inst> Parser::ParseROR() {
    Consume(TokenType::ROR);
    Expect(TokenType::Iden);
    llvm::StringRef r1(tok.ptr, tok.len);
    if (regMap.find(r1) == regMap.end()) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "register");
    }
    int reg1 = regMap[r1];
    Consume(TokenType::Iden);
    Consume(TokenType::COMMA);
    
    Expect(TokenType::Iden);
    llvm::StringRef r2(tok.ptr, tok.len);
    if (regMap.find(r2) == regMap.end()) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "register");
    }
    int reg2 = regMap[r2];
    Consume(TokenType::Iden);
    Consume(TokenType::COMMA);

    Expect(TokenType::Num);
    int cx = tok.numVal;
    Advance();

    auto t = std::make_shared<ROR>();
    t->reg1 = reg1;
    t->reg2 = reg2;
    t->cx = cx;
    return t;
}

std::shared_ptr<Inst> Parser::ParseSHL() {
    Consume(TokenType::SHL);
    Expect(TokenType::Iden);
    llvm::StringRef r1(tok.ptr, tok.len);
    if (regMap.find(r1) == regMap.end()) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "register");
    }
    int reg1 = regMap[r1];
    Consume(TokenType::Iden);
    Consume(TokenType::COMMA);
    
    Expect(TokenType::Iden);
    llvm::StringRef r2(tok.ptr, tok.len);
    if (regMap.find(r2) == regMap.end()) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "register");
    }
    int reg2 = regMap[r2];
    Consume(TokenType::Iden);
    Consume(TokenType::COMMA);

    Expect(TokenType::Num);
    int cx = tok.numVal;
    Advance();

    auto t = std::make_shared<SHL>();
    t->reg1 = reg1;
    t->reg2 = reg2;
    t->cx = cx;
    return t;
}

std::shared_ptr<Inst> Parser::ParseSHR() {
    Consume(TokenType::SHR);
    Expect(TokenType::Iden);
    llvm::StringRef r1(tok.ptr, tok.len);
    if (regMap.find(r1) == regMap.end()) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "register");
    }
    int reg1 = regMap[r1];
    Consume(TokenType::Iden);
    Consume(TokenType::COMMA);
    
    Expect(TokenType::Iden);
    llvm::StringRef r2(tok.ptr, tok.len);
    if (regMap.find(r2) == regMap.end()) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "register");
    }
    int reg2 = regMap[r2];
    Consume(TokenType::Iden);
    Consume(TokenType::COMMA);

    Expect(TokenType::Num);
    int cx = tok.numVal;
    Advance();

    auto t = std::make_shared<SHR>();
    t->reg1 = reg1;
    t->reg2 = reg2;
    t->cx = cx;
    return t;
}

std::shared_ptr<Inst> Parser::ParseJEQ() {
    Consume(TokenType::JEQ);
    if (tok.kind != TokenType::Iden && tok.kind != TokenType::Num) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "symbol or number");
    }

    Expect(TokenType::Iden);
    llvm::StringRef cx = llvm::StringRef(tok.ptr, tok.len);
    Advance();

    auto t = std::make_shared<JEQ>();
    t->cx = cx;
    return t;
}

std::shared_ptr<Inst> Parser::ParseJNE() {
    Consume(TokenType::JNE);
    if (tok.kind != TokenType::Iden && tok.kind != TokenType::Num) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "symbol or number");
    }

    Expect(TokenType::Iden);
    llvm::StringRef cx = llvm::StringRef(tok.ptr, tok.len);
    Advance();

    auto t = std::make_shared<JNE>();
    t->cx = cx;
    return t;
}

std::shared_ptr<Inst> Parser::ParseJLT() {
    Consume(TokenType::JLT);
    if (tok.kind != TokenType::Iden && tok.kind != TokenType::Num) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "symbol or number");
    }

    Expect(TokenType::Iden);
    llvm::StringRef cx = llvm::StringRef(tok.ptr, tok.len);
    Advance();

    auto t = std::make_shared<JLT>();
    t->cx = cx;
    return t;
}

std::shared_ptr<Inst> Parser::ParseJGT() {
    Consume(TokenType::JGT);
    if (tok.kind != TokenType::Iden && tok.kind != TokenType::Num) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "symbol or number");
    }

    Expect(TokenType::Iden);
    llvm::StringRef cx = llvm::StringRef(tok.ptr, tok.len);
    Advance();

    auto t = std::make_shared<JGT>();
    t->cx = cx;
    return t;
}

std::shared_ptr<Inst> Parser::ParseJLE() {
    Consume(TokenType::JLE);
    if (tok.kind != TokenType::Iden && tok.kind != TokenType::Num) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "symbol or number");
    }

    Expect(TokenType::Iden);
    llvm::StringRef cx = llvm::StringRef(tok.ptr, tok.len);
    Advance();

    auto t = std::make_shared<JLE>();
    t->cx = cx;
    return t;
}

std::shared_ptr<Inst> Parser::ParseJGE() {
    Consume(TokenType::JGE);
    if (tok.kind != TokenType::Iden && tok.kind != TokenType::Num) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "symbol or number");
    }

    Expect(TokenType::Iden);
    llvm::StringRef cx = llvm::StringRef(tok.ptr, tok.len);
    Advance();

    auto t = std::make_shared<JGE>();
    t->cx = cx;
    return t;
}

std::shared_ptr<Inst> Parser::ParseJMP() {
    Consume(TokenType::JMP);
    if (tok.kind != TokenType::Iden && tok.kind != TokenType::Num) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "symbol or number");
    }

    Expect(TokenType::Iden);
    llvm::StringRef cx = llvm::StringRef(tok.ptr, tok.len);
    Advance();

    auto t = std::make_shared<JMP>();
    t->cx = cx;
    return t;
}

std::shared_ptr<Inst> Parser::ParseSWI() {
    Consume(TokenType::SWI);
    if (tok.kind != TokenType::Iden && tok.kind != TokenType::Num) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "symbol or number");
    }

    Expect(TokenType::Num);
    int cx = tok.numVal;
    Advance();

    auto t = std::make_shared<SWI>();
    t->cx = cx;
    return t;
}

std::shared_ptr<Inst> Parser::ParseCALL() {
    Consume(TokenType::CALL);
    if (tok.kind != TokenType::Iden && tok.kind != TokenType::Num) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "symbol or number");
    }

    Expect(TokenType::Iden);
    llvm::StringRef cx = llvm::StringRef(tok.ptr, tok.len);
    Advance();

    auto t = std::make_shared<CALL>();
    t->cx = cx;
    return t;
}

std::shared_ptr<Inst> Parser::ParseRET() {
    Consume(TokenType::RET);
    return std::make_shared<RET>();
}

std::shared_ptr<Inst> Parser::ParseIRET() {
    Consume(TokenType::IRET);
    return std::make_shared<IRET>();
}

std::shared_ptr<Inst> Parser::ParsePUSH() {
    Consume(TokenType::PUSH);
    Expect(TokenType::Iden);
    llvm::StringRef r1(tok.ptr, tok.len);
    if (regMap.find(r1) == regMap.end()) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "register");
    }
    int reg1 = regMap[r1];
    Consume(TokenType::Iden);

    auto t = std::make_shared<PUSH>();
    t->reg1 = reg1;
    return t;
}

std::shared_ptr<Inst> Parser::ParsePOP() {
    Consume(TokenType::POP);
    Expect(TokenType::Iden);
    llvm::StringRef r1(tok.ptr, tok.len);
    if (regMap.find(r1) == regMap.end()) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "register");
    }
    int reg1 = regMap[r1];
    Consume(TokenType::Iden);

    auto t = std::make_shared<POP>();
    t->reg1 = reg1;
    return t;
}

std::shared_ptr<Inst> Parser::ParsePUSHB() {
    Consume(TokenType::PUSHB);
    Expect(TokenType::Iden);
    llvm::StringRef r1(tok.ptr, tok.len);
    if (regMap.find(r1) == regMap.end()) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "register");
    }
    int reg1 = regMap[r1];
    Consume(TokenType::Iden);

    auto t = std::make_shared<PUSHB>();
    t->reg1 = reg1;
    return t;
}

std::shared_ptr<Inst> Parser::ParsePOPB() {
    Consume(TokenType::POPB);
    Expect(TokenType::Iden);
    llvm::StringRef r1(tok.ptr, tok.len);
    if (regMap.find(r1) == regMap.end()) {
        diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "register");
    }
    int reg1 = regMap[r1];
    Consume(TokenType::Iden);

    auto t = std::make_shared<POPB>();
    t->reg1 = reg1;
    return t;
}

std::shared_ptr<Inst> Parser::ParseRESW() {
    Consume(TokenType::RESW);
    Expect(TokenType::Num);
    int num = tok.numVal;
    Consume(TokenType::Num);

    auto t = std::make_shared<RESW>();
    t->num = num;
    t->SetSize(num * 4);
    return t;
}

std::shared_ptr<Inst> Parser::ParseRESB() {
    Consume(TokenType::RESB);
    Expect(TokenType::Num);
    int num = tok.numVal;
    Consume(TokenType::Num);

    auto t = std::make_shared<RESB>();
    t->num = num;
    t->SetSize(num);
    return t;
}

std::shared_ptr<Inst> Parser::ParseWORD() {
    Consume(TokenType::WORD);
    llvm::SmallVector<std::variant<WORD::symbol, int>> args;
    int i = 0;
    int unitSize = 4;
    int size = 0;
    do {
        if (i > 0) {
            Consume(TokenType::COMMA);
        }
        if (tok.kind != TokenType::Iden && tok.kind != TokenType::Num) {
            diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "symbol or number");
        }
        std::variant<WORD::symbol, int> cx;
        if (tok.kind == TokenType::Iden) {
            WORD::symbol sb;
            sb.s = llvm::StringRef(tok.ptr, tok.len);
            cx = sb;
            size += unitSize;
        }else {
            cx = tok.numVal;
            size += unitSize;
        }
        args.push_back(cx);
        ++i;
        Advance();
    }while (tok.kind == TokenType::COMMA);

    auto t = std::make_shared<WORD>();
    t->args = args;
    t->SetSize(size);
    return t;
}

std::shared_ptr<Inst> Parser::ParseBYTE() {
    Consume(TokenType::BYTE);
    llvm::SmallVector<std::variant<BYTE::symbol, llvm::StringRef, int>> args;
    int i = 0;
    int unitSize = 1;
    int size = 0;
    do {
        if (i > 0) {
            Consume(TokenType::COMMA);
        }
        if (tok.kind != TokenType::String && tok.kind != TokenType::Num) {
            diagEngine.Report(llvm::SMLoc::getFromPointer(tok.ptr), diag::err_expected, "string or number");
        }
        std::variant<BYTE::symbol, llvm::StringRef, int> cx;
        if (tok.kind == TokenType::Iden) {
            BYTE::symbol sb;
            sb.s = llvm::StringRef(tok.ptr, tok.len);
            cx = sb;
            size += unitSize;
        }else if (tok.kind == TokenType::String) {
            cx = llvm::StringRef(tok.ptr, tok.len);
            size += (unitSize * tok.len);
        }else {
            cx = tok.numVal;
            size += unitSize;
        }
        args.push_back(cx);
        ++i;
        Advance();
    }while (tok.kind == TokenType::COMMA);

    auto t = std::make_shared<BYTE>();
    t->args = args;
    t->SetSize(size);
    return t;
}

bool Parser::Expect(TokenType tokenType) {
    if (tok.kind == tokenType) {
        return true;
    }
    diagEngine.Report(
        llvm::SMLoc::getFromPointer(tok.ptr), 
        diag::err_expected, 
        lexer.GetSpellingText(tokenType), 
        llvm::StringRef(tok.ptr, tok.len));
    return false;
}

bool Parser::Consume(TokenType tokenType) {
    if (Expect(tokenType)) {
        Advance();
        return true;
    }
    return false;
}

void Parser::Advance() {
    lexer.NextToken(tok);
}

bool Parser::IsInstName() {
    if (tok.kind != TokenType::Iden &&
        tok.kind != TokenType::COMMA &&
        tok.kind != TokenType::COLON && 
        tok.kind != TokenType::Num &&
        tok.kind != TokenType::String &&
        tok.kind != TokenType::Eof) {
            return true;
        }
    return false;
}

void Parser::InitialRegMap() {
    regMap = {
        {"R0", 0},
        {"R1", 1},
        {"R2", 2},
        {"R3", 3},
        {"R4", 4},
        {"R5", 5},
        {"R6", 6},
        {"R7", 7},
        {"R8", 8},
        {"R9", 9},
        {"R10", 10},
        {"R11", 11},
        {"R12", 12},
        {"R13", 13},
        {"R14", 14},
        {"R15", 15}
    };
}
}