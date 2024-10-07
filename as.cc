#include "as.h"
namespace cpu0 {
const std::string & AS::Assember() {
    for (auto &inst : instVec_) {
        ra = rb = rc = cx = 0;
        pc = inst->Addr() + 4;

        inst->Accept(this);

        out_ += inst->ObjCode();
    }
    return out_;
}

void AS::VisitLD(LD *t) {
    ra = t->reg1;
    rb = t->reg2;

    std::visit(overload{
        [&](llvm::StringRef symbol) {
            auto inst = symbolTable_.find(symbol)->getSecond();
            cx = inst->Addr() - pc;
        },
        [&](int n) {
            cx = n;
        }
    }, t->cx);

    std::string obj = hex(t->op, 2) + hex(ra, 1) + hex(rb, 1) + hex(cx, 4);
    t->SetObjCode(obj);
}
void AS::VisitST(ST *t) {
    ra = t->reg1;
    rb = t->reg2;

    std::visit(overload{
        [&](llvm::StringRef symbol) {
            auto inst = symbolTable_.find(symbol)->getSecond();
            cx = inst->Addr() - pc;
        },
        [&](int n) {
            cx = n;
        }
    }, t->cx);

    std::string obj = hex(t->op, 2) + hex(ra, 1) + hex(rb, 1) + hex(cx, 4);
    t->SetObjCode(obj);
}
void AS::VisitLDB(LDB *t) {
    ra = t->reg1;
    rb = t->reg2;

    std::visit(overload{
        [&](llvm::StringRef symbol) {
            auto inst = symbolTable_.find(symbol)->getSecond();
            cx = inst->Addr() - pc;
        },
        [&](int n) {
            cx = n;
        }
    }, t->cx);

    std::string obj = hex(t->op, 2) + hex(ra, 1) + hex(rb, 1) + hex(cx, 4);
    t->SetObjCode(obj);
}
void AS::VisitSTB(STB *t) {
    ra = t->reg1;
    rb = t->reg2;

    std::visit(overload{
        [&](llvm::StringRef symbol) {
            auto inst = symbolTable_.find(symbol)->getSecond();
            cx = inst->Addr() - pc;
        },
        [&](int n) {
            cx = n;
        }
    }, t->cx);

    std::string obj = hex(t->op, 2) + hex(ra, 1) + hex(rb, 1) + hex(cx, 4);
    t->SetObjCode(obj);    
}
void AS::VisitLDR(LDR *t) {
    ra = t->reg1;
    rb = t->reg2;
    rc = t->reg3;
    std::string obj = hex(t->op, 2) + hex(ra, 1) + hex(rb, 1) + hex(rc, 1) + hex(cx, 3);
    t->SetObjCode(obj); 
}
void AS::VisitSTR(STR *t) {
    ra = t->reg1;
    rb = t->reg2;
    rc = t->reg3;
    std::string obj = hex(t->op, 2) + hex(ra, 1) + hex(rb, 1) + hex(rc, 1) + hex(cx, 3);
    t->SetObjCode(obj);
}
void AS::VisitLBR(LBR *t) {
    ra = t->reg1;
    rb = t->reg2;
    rc = t->reg3;
    std::string obj = hex(t->op, 2) + hex(ra, 1) + hex(rb, 1) + hex(rc, 1) + hex(cx, 3);
    t->SetObjCode(obj);
}
void AS::VisitSBR(SBR *t) {
    ra = t->reg1;
    rb = t->reg2;
    rc = t->reg3;
    std::string obj = hex(t->op, 2) + hex(ra, 1) + hex(rb, 1) + hex(rc, 1) + hex(cx, 3);
    t->SetObjCode(obj);
}
void AS::VisitLDI(LDI *t) {
    ra = t->reg1;
    cx = t->num;
    std::string obj = hex(t->op, 2) + hex(ra, 1) + hex(rb, 1) + hex(cx, 4);
    t->SetObjCode(obj);
}
void AS::VisitCMP(CMP *t) {
    ra = t->reg1;
    rb = t->reg2;
    std::string obj = hex(t->op, 2) + hex(ra, 1) + hex(rb, 1) + hex(rc, 1) + hex(cx, 3);
    t->SetObjCode(obj);
}
void AS::VisitMOV(MOV *t) {
    ra = t->reg1;
    rb = t->reg2;
    std::string obj = hex(t->op, 2) + hex(ra, 1) + hex(rb, 1) + hex(rc, 1) + hex(cx, 3);
    t->SetObjCode(obj);
}
void AS::VisitADD(ADD *t) {
    ra = t->reg1;
    rb = t->reg2;
    rc = t->reg3;
    std::string obj = hex(t->op, 2) + hex(ra, 1) + hex(rb, 1) + hex(rc, 1) + hex(cx, 3);
    t->SetObjCode(obj);    
}
void AS::VisitSUB(SUB *t) {
    ra = t->reg1;
    rb = t->reg2;
    rc = t->reg3;
    std::string obj = hex(t->op, 2) + hex(ra, 1) + hex(rb, 1) + hex(rc, 1) + hex(cx, 3);
    t->SetObjCode(obj);    
}
void AS::VisitMUL(MUL *t) {
    ra = t->reg1;
    rb = t->reg2;
    rc = t->reg3;
    std::string obj = hex(t->op, 2) + hex(ra, 1) + hex(rb, 1) + hex(rc, 1) + hex(cx, 3);
    t->SetObjCode(obj);    
}
void AS::VisitDIV(DIV *t) {
    ra = t->reg1;
    rb = t->reg2;
    rc = t->reg3;
    std::string obj = hex(t->op, 2) + hex(ra, 1) + hex(rb, 1) + hex(rc, 1) + hex(cx, 3);
    t->SetObjCode(obj);    
}
void AS::VisitAND(AND *t) {
    ra = t->reg1;
    rb = t->reg2;
    rc = t->reg3;
    std::string obj = hex(t->op, 2) + hex(ra, 1) + hex(rb, 1) + hex(rc, 1) + hex(cx, 3);
    t->SetObjCode(obj);    
}
void AS::VisitOR(OR *t) {
    ra = t->reg1;
    rb = t->reg2;
    rc = t->reg3;
    std::string obj = hex(t->op, 2) + hex(ra, 1) + hex(rb, 1) + hex(rc, 1) + hex(cx, 3);
    t->SetObjCode(obj);    
}
void AS::VisitXOR(XOR *t) {
    ra = t->reg1;
    rb = t->reg2;
    rc = t->reg3;
    std::string obj = hex(t->op, 2) + hex(ra, 1) + hex(rb, 1) + hex(rc, 1) + hex(cx, 3);
    t->SetObjCode(obj);
}
void AS::VisitADDI(ADDI *t) {
    ra = t->reg1;
    rb = t->reg2;
    cx = t->cx;
    std::string obj = hex(t->op, 2) + hex(ra, 1) + hex(rb, 1) + hex(rc, 1) + hex(cx, 3);
    t->SetObjCode(obj);    
}
void AS::VisitROL(ROL *t) {
    ra = t->reg1;
    rb = t->reg2;
    cx = t->cx;
    std::string obj = hex(t->op, 2) + hex(ra, 1) + hex(rb, 1) + hex(rc, 1) + hex(cx, 3);
    t->SetObjCode(obj); 
}
void AS::VisitROR(ROR *t) {
    ra = t->reg1;
    rb = t->reg2;
    cx = t->cx;
    std::string obj = hex(t->op, 2) + hex(ra, 1) + hex(rb, 1) + hex(rc, 1) + hex(cx, 3);
    t->SetObjCode(obj); 
}
void AS::VisitSHL(SHL *t) {
    ra = t->reg1;
    rb = t->reg2;
    cx = t->cx;
    std::string obj = hex(t->op, 2) + hex(ra, 1) + hex(rb, 1) + hex(rc, 1) + hex(cx, 3);
    t->SetObjCode(obj); 
}
void AS::VisitSHR(SHR *t) {
    ra = t->reg1;
    rb = t->reg2;
    cx = t->cx;
    std::string obj = hex(t->op, 2) + hex(ra, 1) + hex(rb, 1) + hex(rc, 1) + hex(cx, 3);
    t->SetObjCode(obj); 
}
void AS::VisitJEQ(JEQ *t) {
    auto inst = symbolTable_.find(t->cx)->getSecond();
    cx = inst->Addr() - pc;
    std::string obj = hex(t->op, 2) + hex(cx, 6);
    t->SetObjCode(obj); 
}
void AS::VisitJNE(JNE *t) {
    auto inst = symbolTable_.find(t->cx)->getSecond();
    cx = inst->Addr() - pc;
    std::string obj = hex(t->op, 2) + hex(cx, 6);
    t->SetObjCode(obj);     
}
void AS::VisitJLT(JLT *t) {
    auto inst = symbolTable_.find(t->cx)->getSecond();
    cx = inst->Addr() - pc;
    std::string obj = hex(t->op, 2) + hex(cx, 6);
    t->SetObjCode(obj);     
}
void AS::VisitJGT(JGT *t) {
    auto inst = symbolTable_.find(t->cx)->getSecond();
    cx = inst->Addr() - pc;
    std::string obj = hex(t->op, 2) + hex(cx, 6);
    t->SetObjCode(obj); 
}
void AS::VisitJLE(JLE *t) {
    auto inst = symbolTable_.find(t->cx)->getSecond();
    cx = inst->Addr() - pc;
    std::string obj = hex(t->op, 2) + hex(cx, 6);
    t->SetObjCode(obj);     
}
void AS::VisitJGE(JGE *t) {
    auto inst = symbolTable_.find(t->cx)->getSecond();
    cx = inst->Addr() - pc;
    std::string obj = hex(t->op, 2) + hex(cx, 6);
    t->SetObjCode(obj);     
}
void AS::VisitJMP(JMP *t) {
    auto inst = symbolTable_.find(t->cx)->getSecond();
    cx = inst->Addr() - pc;
    std::string obj = hex(t->op, 2) + hex(cx, 6);
    t->SetObjCode(obj);     
}
void AS::VisitSWI(SWI *t) {
    cx = t->cx;
    std::string obj = hex(t->op, 2) + hex(cx, 6);
    t->SetObjCode(obj); 
}
void AS::VisitCALL(CALL *t) {
    auto inst = symbolTable_.find(t->cx)->getSecond();
    cx = inst->Addr() - pc;
    std::string obj = hex(t->op, 2) + hex(cx, 6);
    t->SetObjCode(obj);   
}
void AS::VisitRET(RET *t) {
    std::string obj = hex(t->op, 2) + hex(cx, 6);
    t->SetObjCode(obj); 
}
void AS::VisitIRET(IRET *t) {
    std::string obj = hex(t->op, 2) + hex(cx, 6);
    t->SetObjCode(obj);
}
void AS::VisitPUSH(PUSH *t) {
    ra = t->reg1;
    std::string obj = hex(t->op, 2) + hex(ra, 1) + hex(rb, 1) + hex(rc, 1) + hex(cx, 3);
    t->SetObjCode(obj);    
}
void AS::VisitPOP(POP *t) {
    ra = t->reg1;
    std::string obj = hex(t->op, 2) + hex(ra, 1) + hex(rb, 1) + hex(rc, 1) + hex(cx, 3);
    t->SetObjCode(obj);        
}
void AS::VisitPUSHB(PUSHB *t) {
    ra = t->reg1;
    std::string obj = hex(t->op, 2) + hex(ra, 1) + hex(rb, 1) + hex(rc, 1) + hex(cx, 3);
    t->SetObjCode(obj);        
}
void AS::VisitPOPB(POPB *t) {
    ra = t->reg1;
    std::string obj = hex(t->op, 2) + hex(ra, 1) + hex(rb, 1) + hex(rc, 1) + hex(cx, 3);
    t->SetObjCode(obj);        
}
void AS::VisitRESW(RESW *t) {
    std::string obj = dup('0', t->Size() * 2);
    t->SetObjCode(obj);        
}
void AS::VisitRESB(RESB *t) {
    std::string obj = dup('0', t->Size() * 2);
    t->SetObjCode(obj);   
}
void AS::VisitWORD(WORD *t) {
    std::string obj;
    for (auto &arg : t->args) {
        std::visit(overload{
            [&](const WORD::symbol &symbol) {
                auto inst = symbolTable_.find(symbol.s)->getSecond();
                obj += hex(inst->Addr(), 4 * 2);
            },
            [&](int n) {
                obj += hex(n, 4 * 2);
            }
        }, arg);
    }
    t->SetObjCode(obj);   
}
void AS::VisitBYTE(BYTE *t) {
    std::string obj;
    for (auto &arg : t->args) {
        std::visit(overload{
            [&](const BYTE::symbol &symbol) {
                auto inst = symbolTable_.find(symbol.s)->getSecond();
                obj += hex(inst->Addr(), 2);
            },
            [&](llvm::StringRef str) {
                obj += str2hex(str);
            },
            [&](int n) {
                obj += hex(n, 2);
            }
        }, arg);
    }
    t->SetObjCode(obj);   
}
}