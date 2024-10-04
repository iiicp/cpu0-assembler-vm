#pragma once
#include <variant>
#include "common.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/StringMap.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/Support/raw_ostream.h"

template<typename... Ts>
struct overload : Ts...
{
    using Ts::operator()...;
};

template<typename... Ts>
overload(Ts...) -> overload<Ts...>;
namespace cpu0 {
class LD;
class ST;
class LDB;
class STB;
class LDR;
class STR;
class LBR;
class SBR;
class LDI;
class CMP;
class MOV;
class ADD;
class SUB;
class MUL;
class DIV;
class AND;
class OR;
class XOR;
class ADDI;
class ROL;
class ROR;
class SHL;
class SHR;
class JEQ;
class JNE;
class JLT;
class JGT;
class JLE;
class JGE;
class JMP;
class SWI;
class CALL;
class RET;
class IRET;
class PUSH;
class POP;
class PUSHB;
class POPB;
class RESW;
class RESB;
class WORD;
class BYTE;

class Visitor {
public:
    virtual ~Visitor() {}
    virtual void VisitLD(LD *t) = 0;
    virtual void VisitST(ST *t) = 0;
    virtual void VisitLDB(LDB *t) = 0;
    virtual void VisitSTB(STB *t) = 0;
    virtual void VisitLDR(LDR *t) = 0;
    virtual void VisitSTR(STR *t) = 0;
    virtual void VisitLBR(LBR *t) = 0;
    virtual void VisitSBR(SBR *t) = 0;
    virtual void VisitLDI(LDI *t) = 0;
    virtual void VisitCMP(CMP *t) = 0;
    virtual void VisitMOV(MOV *t) = 0;
    virtual void VisitADD(ADD *t) = 0;
    virtual void VisitSUB(SUB *t) = 0;
    virtual void VisitMUL(MUL *t) = 0;
    virtual void VisitDIV(DIV *t) = 0;
    virtual void VisitAND(AND *t) = 0;
    virtual void VisitOR(OR *t) = 0;
    virtual void VisitXOR(XOR *t) = 0;
    virtual void VisitADDI(ADDI *t) = 0;
    virtual void VisitROL(ROL *t) = 0;
    virtual void VisitROR(ROR *t) = 0;
    virtual void VisitSHL(SHL *t) = 0;
    virtual void VisitSHR(SHR *t) = 0;
    virtual void VisitJEQ(JEQ *t) = 0;
    virtual void VisitJNE(JNE *t) = 0;
    virtual void VisitJLT(JLT *t) = 0;
    virtual void VisitJGT(JGT *t) = 0;
    virtual void VisitJLE(JLE *t) = 0;
    virtual void VisitJGE(JGE *t) = 0;
    virtual void VisitJMP(JMP *t) = 0;
    virtual void VisitSWI(SWI *t) = 0;
    virtual void VisitCALL(CALL *t) = 0;
    virtual void VisitRET(RET *t) = 0;
    virtual void VisitIRET(IRET *t) = 0;
    virtual void VisitPUSH(PUSH *t) = 0;
    virtual void VisitPOP(POP *t) = 0;
    virtual void VisitPUSHB(PUSHB *t) = 0;
    virtual void VisitPOPB(POPB *t) = 0;
    virtual void VisitRESW(RESW *t) = 0;
    virtual void VisitRESB(RESB *t) = 0;
    virtual void VisitWORD(WORD *t) = 0;
    virtual void VisitBYTE(BYTE *t) = 0;
};

class Inst {
public:
    enum Kind {
        Inst_LD,     // L | 0x00 | load word     | LD Ra,[Rb+Cx]  | Ra = [Rb + Cx]
        Inst_ST,     // L | 0x01 | store word    | ST Ra,[Rb+Cx]  | [Rb+Cx] = Ra;
        Inst_LDB,    // L | 0x02 | load byte     | LDB Ra,[Rb+Cx] | Ra =(byte)[Rb+Cx]
        Inst_STB,    // L | 0x03 | store byte    | STB Ra,[Rb+Cx] | [Rb+Cx] = (byte)Ra
        Inst_LDR,    // L | 0x04 | LD的寄存器版本  | LDR Ra,[Rb+Rc] | Ra = [Rb+Rc]
        Inst_STR,    // L | 0x05 | ST的寄存器版本  | STR Ra,[Rb+Rc] | [Rb+Rc] = Ra
        Inst_LBR,    // L | 0x06 | LDB的寄存器版本 | LBR Ra,[Rb+Rc] | Ra = (byte)[Rb+Rc]
        Inst_SBR,    // L | 0x07 | SBR的寄存器版本 | SBR Ra,[Rb+Rc] | [Rb+Rc] = (byte)Ra
        Inst_LDI,    // L | 0x08 | 载入常数        | LDI Ra, Cx    | Ra = Cx
        Inst_CMP,    // A | 0x10 | 比较           | CMP Ra, Rb     | SW = Ra >=< Rb
        Inst_MOV,    // A | 0x12 | 移动           | MOV Ra, Rb     | Ra = Rb
        Inst_ADD,    // A | 0x13 | 加法           | ADD Ra, Rb, Rc | Ra = Rb + Rc
        Inst_SUB,    // A | 0x14 | 减法           | SUB Ra, Rb, Rc | Ra = Rb - Rc
        Inst_MUL,    // A | 0x15 | 乘法           | MUL Ra, Rb, Rc | Ra = Rb * Rc
        Inst_DIV,    // A | 0x16 | 除法           | DIV Ra, Rb, Rc | Ra = Rb / Rc
        Inst_AND,    // A | 0x18 | 按位与         | AND Ra, Rb, Rc | Ra = Rb & Rc
        Inst_OR,     // A | 0x19 | 按位或         | OR Ra, Rb, Rc  | Ra = Rb | Rc
        Inst_XOR,    // A | 0x1A | 按位异或        | XOR Ra,Rb,Rc  | Ra = Rb ^ Rc
        Inst_ADDI,   // A | 0x1B | 常数加法        | ADDI Ra,Rb,Cx | Ra = Rb+Cx
        Inst_ROL,    // A | 0x1C | 循环左移        | ROL Ra,Rb,Cx  | Ra = Rb rol Cx
        Inst_ROR,    // A | 0x1D | 循环右移        | ROR Ra,Rb,Cx  | Ra = Rb ror Cx
        Inst_SHL,    // A | 0x1E | 向左移位        | SHL Ra,Rb,Cx  | Ra = Rb << Cx
        Inst_SHR,    // A | 0x1F | 向右移位        | SHR Ra,Rb,Cx  | Ra = Rb >> Cx
        Inst_JEQ,    // J | 0x20 | 相等跳转        | JEQ Cx        | if SW(=) PC = PC+Cx
        Inst_JNE,    // J | 0x21 | 不等跳转        | JNE Cx        | if SW(!=) PC=PC+Cx
        Inst_JLT,    // J | 0x22 | 小于跳转        | JLT Cx        | if SW(<) PC= PC+Cx
        Inst_JGT,    // J | 0x23 | 大于跳转        | JGT Cx        | if SW(>) PC=PC+Cx
        Inst_JLE,    // J | 0x24 | 小于等于跳转     | JLE Cx        | if SW(<=) PC=PC+Cx
        Inst_JGE,    // J | 0x25 | 大于等于跳转     | JGE Cx        | if SW(>=) PC=PC+Cx
        Inst_JMP,    // J | 0x26 | 无条件跳转       | JMP Cx       | PC=PC+Cx
        Inst_SWI,    // J | 0x2A | 软件中断         | SWI Cx       | LR=PC; PC=Cx; INT=1
        Inst_CALL,   // J | 0x2B | 调用            | CALL Cx      | LR=PC; PC=PC+Cx
        Inst_RET,    // J | 0x2C | 返回            | RET          | PC=LR
        Inst_IRET,   // J | 0x2D | 中断返回         | IRET         | PC=LR; INT=0
        Inst_PUSH,   // A | 0x30 | push word       | PUSH Ra      | SP-=4;[SP]=Ra
        Inst_POP,    // A | 0x31 | pop word        | POP Ra       | Ra=[SP]; SP+=4
        Inst_PUSHB,  // A | 0x32 | push byte       | PUSH Ra      | SP--;[SP]=(byte)Ra
        Inst_POPB,   // A | 0x33 | pop byte        | POP Ra       | Ra=(byte)[SP]; SP++
        Inst_RESW,   // D | 0xF0 | 保留n个word      
        Inst_RESB,   // D | 0xF1 | 保留n个byte    
        Inst_WORD,   // D | 0xF2 | 定义有初值的整数变量
        Inst_BYTE,   // D | 0xf3 | 定义有初值的字节变量或者字符串
    };
    Inst(Kind kind): kind(kind) {}
    virtual ~Inst(){};
    const Kind GetKind() const {return kind;}

    void SetAddr(uint32_t addr) {addr_ = addr;}
    uint32_t Addr() const {return addr_;}

    void SetLable(llvm::StringRef label) {label_ = label;}
    llvm::StringRef Label() const {return label_;}

    void SetSize(uint32_t size) {size_ = size;}
    uint32_t Size() const { return size_; }

    void SetObjCode(const std::string &objCode) {objCode_ = objCode;}
    const std::string &ObjCode() const {
        return objCode_;
    }

    virtual void Accept(Visitor *v) {}
    virtual void Dump() {}

private:
    const Kind kind;
    uint32_t addr_;
    llvm::StringRef label_;
    uint32_t size_{4};
    std::string objCode_;
};

class LD : public Inst{
public:
    int op{0x00};
    int reg1;
    int reg2;
    std::variant<llvm::StringRef, int> cx;
    LD() : Inst(Inst_LD) {}
    static bool classof(const Inst *st) {
        return st->GetKind() == Inst_LD;
    }
    void Accept(Visitor *v) override {
        return v->VisitLD(this);
    }
    void Dump() override {
        llvm::outs() << hex(Addr(), 8) << ": ";
        Label().size() > 0 ? (llvm::outs() << Label() << ": \t") : llvm::outs() << "\t";
        llvm::outs() << "LD \t" << "R" << reg1 << ", R" << reg2 << ", ";
        std::visit([&](auto &&v) {
            llvm::outs() << v;
        }, cx);
        if (ObjCode().size() > 0) {
            llvm::outs() << "\t\t=> " << ObjCode() << "\n";
        }else {
            llvm::outs() << "\n";
        }          
    }
};

class ST : public Inst{
public:
    int op{0x01};
    int reg1;
    int reg2;
    std::variant<llvm::StringRef, int> cx;
    ST() : Inst(Inst_ST) {}
    static bool classof(const Inst *st) {
        return st->GetKind() == Inst_ST;
    }
    void Accept(Visitor *v) override {
        return v->VisitST(this);
    }
    void Dump() override {
        llvm::outs() << hex(Addr(), 8) << ": ";
        Label().size() > 0 ? (llvm::outs() << Label() << ": \t") : llvm::outs() << "\t";
        llvm::outs() << "ST \t" << "R" << reg1 << ", R" << reg2 << ", ";
        std::visit([&](auto &&v) {
            llvm::outs() << v;
        }, cx);
        if (ObjCode().size() > 0) {
            llvm::outs() << "\t\t=> " << ObjCode() << "\n";
        }else {
            llvm::outs() << "\n";
        }          
    }
};

class LDB : public Inst{
public:
    int op{0x02};
    int reg1;
    int reg2;
    std::variant<llvm::StringRef, int> cx;
    LDB() : Inst(Inst_LDB) {}
    static bool classof(const Inst *st) {
        return st->GetKind() == Inst_LDB;
    }
    void Accept(Visitor *v) override {
        return v->VisitLDB(this);
    }
    void Dump() override {
        llvm::outs() << hex(Addr(), 8) << ": ";
        Label().size() > 0 ? (llvm::outs() << Label() << ": \t") : llvm::outs() << "\t";
        llvm::outs() <<  "LDB \t" << "R" << reg1 << ", R" << reg2 << ", ";
        std::visit([&](auto &&v) {
            llvm::outs() << v;
        }, cx);
        if (ObjCode().size() > 0) {
            llvm::outs() << "\t\t=> " << ObjCode() << "\n";
        }else {
            llvm::outs() << "\n";
        }  
    }
};

class STB : public Inst{
public:
    int op{0x03};
    int reg1;
    int reg2;
    std::variant<llvm::StringRef, int> cx;
    STB() : Inst(Inst_STB) {}
    static bool classof(const Inst *st) {
        return st->GetKind() == Inst_STB;
    }
    void Accept(Visitor *v) override {
        return v->VisitSTB(this);
    }   
    void Dump() override {
        llvm::outs() << hex(Addr(), 8) << ": ";
        Label().size() > 0 ? (llvm::outs() << Label() << ": \t") : llvm::outs() << "\t";
        llvm::outs() << "STB \t" << "R" << reg1 << ", R" << reg2 << ", ";
        std::visit([&](auto &&v) {
            llvm::outs() << v << "\n";
        }, cx);
    }     
};

class LDR : public Inst{
public:
    int op{0x04};
    int reg1;
    int reg2;
    int reg3;
    LDR() : Inst(Inst_LDR) {}
    static bool classof(const Inst *st) {
        return st->GetKind() == Inst_LDR;
    }
    void Accept(Visitor *v) override {
        return v->VisitLDR(this);
    }  
    void Dump() override {
        llvm::outs() << hex(Addr(), 8) << ": ";
        Label().size() > 0 ? (llvm::outs() << Label() << ": \t") : llvm::outs() << "\t";
        llvm::outs() << "LDR \t" << "R" << reg1 << ", R" << reg2 << ", R" << reg3;
        if (ObjCode().size() > 0) {
            llvm::outs() << "\t\t=> " << ObjCode() << "\n";
        }else {
            llvm::outs() << "\n";
        }          
    }       
};

class STR : public Inst{
public:
    int op{0x05};
    int reg1;
    int reg2;
    int reg3;
    STR() : Inst(Inst_STR) {}
    static bool classof(const Inst *st) {
        return st->GetKind() == Inst_STR;
    }
    void Accept(Visitor *v) override {
        return v->VisitSTR(this);
    }  
    void Dump() override {
        llvm::outs() << hex(Addr(), 8) << ": ";
        Label().size() > 0 ? (llvm::outs() << Label() << ": \t") : llvm::outs() << "\t";
        llvm::outs() <<  "STR \t" << "R" << reg1 << ", R" << reg2 << ", R" << reg3;
        if (ObjCode().size() > 0) {
            llvm::outs() << "\t\t=> " << ObjCode() << "\n";
        }else {
            llvm::outs() << "\n";
        }          
    }     
};

class LBR : public Inst{
public:
    int op{0x06};
    int reg1;
    int reg2;
    int reg3;
    LBR() : Inst(Inst_LBR) {}
    static bool classof(const Inst *st) {
        return st->GetKind() == Inst_LBR;
    }
    void Accept(Visitor *v) override {
        return v->VisitLBR(this);
    }  
    void Dump() override {
        llvm::outs() << hex(Addr(), 8) << ": ";
        Label().size() > 0 ? (llvm::outs() << Label() << ": \t") : llvm::outs() << "\t";
        llvm::outs() <<  "LBR \t" << "R" << reg1 << ", R" << reg2 << ", R" << reg3;
        if (ObjCode().size() > 0) {
            llvm::outs() << "\t\t=> " << ObjCode() << "\n";
        }else {
            llvm::outs() << "\n";
        }          
    }             
};

class SBR : public Inst{
public:
    int op{0x07};
    int reg1;
    int reg2;
    int reg3;
    SBR() : Inst(Inst_SBR) {}
    static bool classof(const Inst *st) {
        return st->GetKind() == Inst_SBR;
    }
    void Accept(Visitor *v) override {
        return v->VisitSBR(this);
    }    
    void Dump() override {
        llvm::outs() << hex(Addr(), 8) << ": ";
        Label().size() > 0 ? (llvm::outs() << Label() << ": \t") : llvm::outs() << "\t";
        llvm::outs() <<  "SBR \t" << "R" << reg1 << ", R" << reg2 << ", R" << reg3;
        if (ObjCode().size() > 0) {
            llvm::outs() << "\t\t=> " << ObjCode() << "\n";
        }else {
            llvm::outs() << "\n";
        }          
    }     
};

class LDI : public Inst{
public:
    int op{0x08};
    int reg1;
    int num;
    LDI() : Inst(Inst_LDI) {}
    static bool classof(const Inst *st) {
        return st->GetKind() == Inst_LDI;
    }
    void Accept(Visitor *v) override {
        return v->VisitLDI(this);
    }  
    void Dump() override {
        llvm::outs() << hex(Addr(), 8) << ": ";
        Label().size() > 0 ? (llvm::outs() << Label() << ": \t") : llvm::outs() << "\t";
        llvm::outs() <<  "LDI \t" << "R" << reg1 << ", " << num;
        if (ObjCode().size() > 0) {
            llvm::outs() << "\t\t=> " << ObjCode() << "\n";
        }else {
            llvm::outs() << "\n";
        }          
    }         
};

class CMP : public Inst{
public:
    int op{0x10};
    int reg1;
    int reg2;
    CMP() : Inst(Inst_CMP) {}
    static bool classof(const Inst *st) {
        return st->GetKind() == Inst_CMP;
    }
    void Accept(Visitor *v) override {
        return v->VisitCMP(this);
    } 
    void Dump() override {
        llvm::outs() << hex(Addr(), 8) << ": ";
        Label().size() > 0 ? (llvm::outs() << Label() << ": \t") : llvm::outs() << "\t";
        llvm::outs() <<  "CMP \t" << "R" << reg1 << ", R" << reg2;
        if (ObjCode().size() > 0) {
            llvm::outs() << "\t\t=> " << ObjCode() << "\n";
        }else {
            llvm::outs() << "\n";
        }          
    }          
};

class MOV : public Inst{
public:
    int op{0x12};
    int reg1;
    int reg2;
    MOV() : Inst(Inst_MOV) {}
    static bool classof(const Inst *st) {
        return st->GetKind() == Inst_MOV;
    }
    void Accept(Visitor *v) override {
        return v->VisitMOV(this);
    }     
    void Dump() override {
        llvm::outs() << hex(Addr(), 8) << ": ";
        Label().size() > 0 ? (llvm::outs() << Label() << ": \t") : llvm::outs() << "\t";
        llvm::outs() <<  "MOV \t" << "R" << reg1 << ", R" << reg2;
        if (ObjCode().size() > 0) {
            llvm::outs() << "\t\t=> " << ObjCode() << "\n";
        }else {
            llvm::outs() << "\n";
        }          
    }       
};

class ADD : public Inst{
public:
    int op{0x13};
    int reg1;
    int reg2;
    int reg3;
    ADD() : Inst(Inst_ADD) {}
    static bool classof(const Inst *st) {
        return st->GetKind() == Inst_ADD;
    }
    void Accept(Visitor *v) override {
        return v->VisitADD(this);
    }    
    void Dump() override {
        llvm::outs() << hex(Addr(), 8) << ": ";
        Label().size() > 0 ? (llvm::outs() << Label() << ": \t") : llvm::outs() << "\t";
        llvm::outs() <<  "ADD \t" << "R" << reg1 << ", R" << reg2 << ", R" << reg3;
        if (ObjCode().size() > 0) {
            llvm::outs() << "\t\t=> " << ObjCode() << "\n";
        }else {
            llvm::outs() << "\n";
        }          
    }       
};

class SUB : public Inst{
public:
    int op{0x14};
    int reg1;
    int reg2;
    int reg3;
    SUB() : Inst(Inst_SUB) {}
    static bool classof(const Inst *st) {
        return st->GetKind() == Inst_SUB;
    }
    void Accept(Visitor *v) override {
        return v->VisitSUB(this);
    } 
    void Dump() override {
        llvm::outs() << hex(Addr(), 8) << ": ";
        Label().size() > 0 ? (llvm::outs() << Label() << ": \t") : llvm::outs() << "\t";
        llvm::outs() <<  "SUB \t" << "R" << reg1 << ", R" << reg2 << ", R" << reg3;
        if (ObjCode().size() > 0) {
            llvm::outs() << "\t\t=> " << ObjCode() << "\n";
        }else {
            llvm::outs() << "\n";
        }          
    }              
};

class MUL : public Inst{
public:
    int op{0x15};
    int reg1;
    int reg2;
    int reg3;
    MUL() : Inst(Inst_MUL) {}
    static bool classof(const Inst *st) {
        return st->GetKind() == Inst_MUL;
    }
    void Accept(Visitor *v) override {
        return v->VisitMUL(this);
    } 
    void Dump() override {
        llvm::outs() << hex(Addr(), 8) << ": ";
        Label().size() > 0 ? (llvm::outs() << Label() << ": \t") : llvm::outs() << "\t";
        llvm::outs() <<  "MUL \t" << "R" << reg1 << ", R" << reg2 << ", R" << reg3;
        if (ObjCode().size() > 0) {
            llvm::outs() << "\t\t=> " << ObjCode() << "\n";
        }else {
            llvm::outs() << "\n";
        }          
    }        
};

class DIV : public Inst{
public:
    int op{0x16};
    int reg1;
    int reg2;
    int reg3;
    DIV() : Inst(Inst_DIV) {}
    static bool classof(const Inst *st) {
        return st->GetKind() == Inst_DIV;
    }
    void Accept(Visitor *v) override {
        return v->VisitDIV(this);
    } 
    void Dump() override {
        llvm::outs() << hex(Addr(), 8) << ": ";
        Label().size() > 0 ? (llvm::outs() << Label() << ": \t") : llvm::outs() << "\t";
        llvm::outs() <<  "DIV \t" << "R" << reg1 << ", R" << reg2 << ", R" << reg3;
        if (ObjCode().size() > 0) {
            llvm::outs() << "\t\t=> " << ObjCode() << "\n";
        }else {
            llvm::outs() << "\n";
        }          
    }        
};

class AND : public Inst{
public:
    int op{0x18};
    int reg1;
    int reg2;
    int reg3;
    AND() : Inst(Inst_AND) {}
    static bool classof(const Inst *st) {
        return st->GetKind() == Inst_AND;
    }
    void Accept(Visitor *v) override {
        return v->VisitAND(this);
    } 
    void Dump() override {
        llvm::outs() << hex(Addr(), 8) << ": ";
        Label().size() > 0 ? (llvm::outs() << Label() << ": \t") : llvm::outs() << "\t";
        llvm::outs() <<  "AND \t" << "R" << reg1 << ", R" << reg2 << ", R" << reg3;
        if (ObjCode().size() > 0) {
            llvm::outs() << "\t\t=> " << ObjCode() << "\n";
        }else {
            llvm::outs() << "\n";
        }          
    }        
};

class OR : public Inst{
public:
    int op{0x19};
    int reg1;
    int reg2;
    int reg3;
    OR() : Inst(Inst_OR) {}
    static bool classof(const Inst *st) {
        return st->GetKind() == Inst_OR;
    }
    void Accept(Visitor *v) override {
        return v->VisitOR(this);
    }  
    void Dump() override {
        llvm::outs() << hex(Addr(), 8) << ": ";
        Label().size() > 0 ? (llvm::outs() << Label() << ": \t") : llvm::outs() << "\t";
        llvm::outs() <<  "OR \t" << "R" << reg1 << ", R" << reg2 << ", R" << reg3;
        if (ObjCode().size() > 0) {
            llvm::outs() << "\t\t=> " << ObjCode() << "\n";
        }else {
            llvm::outs() << "\n";
        }          
    }       
};

class XOR : public Inst{
public:
    int op{0x1A};
    int reg1;
    int reg2;
    int reg3;
    XOR() : Inst(Inst_XOR) {}
    static bool classof(const Inst *st) {
        return st->GetKind() == Inst_XOR;
    }
    void Accept(Visitor *v) override {
        return v->VisitXOR(this);
    } 
    void Dump() override {
        llvm::outs() << hex(Addr(), 8) << ": ";
        Label().size() > 0 ? (llvm::outs() << Label() << ": \t") : llvm::outs() << "\t";
        llvm::outs() <<  "XOR \t" << "R" << reg1 << ", R" << reg2 << ", R" << reg3;
        if (ObjCode().size() > 0) {
            llvm::outs() << "\t\t=> " << ObjCode() << "\n";
        }else {
            llvm::outs() << "\n";
        }          
    }           
};

class ADDI : public Inst{
public:
    int op{0x1B};
    int reg1;
    int reg2;
    int cx;
    ADDI() : Inst(Inst_ADDI) {}
    static bool classof(const Inst *st) {
        return st->GetKind() == Inst_ADDI;
    }
    void Accept(Visitor *v) override {
        return v->VisitADDI(this);
    } 
    void Dump() override {
        llvm::outs() << hex(Addr(), 8) << ": ";
        Label().size() > 0 ? (llvm::outs() << Label() << ": \t") : llvm::outs() << "\t";
        llvm::outs() <<  "ADDI \t" << "R" << reg1 << ", R" << reg2 << ", " << cx;
        if (ObjCode().size() > 0) {
            llvm::outs() << "\t\t=> " << ObjCode() << "\n";
        }else {
            llvm::outs() << "\n";
        }          
    }        
};

class ROL : public Inst{
public:
    int op{0x1C};
    int reg1;
    int reg2;
    int cx;
    ROL() : Inst(Inst_ROL) {}
    static bool classof(const Inst *st) {
        return st->GetKind() == Inst_ROL;
    }
    void Accept(Visitor *v) override {
        return v->VisitROL(this);
    }
    void Dump() override {
        llvm::outs() << hex(Addr(), 8) << ": ";
        Label().size() > 0 ? (llvm::outs() << Label() << ": \t") : llvm::outs() << "\t";
        llvm::outs() <<   "ROL \t" << "R" << reg1 << ", R" << reg2 << ", " << cx;
        if (ObjCode().size() > 0) {
            llvm::outs() << "\t\t=> " << ObjCode() << "\n";
        }else {
            llvm::outs() << "\n";
        }          
    }         
};

class ROR : public Inst{
public:
    int op{0x1D};
    int reg1;
    int reg2;
    int cx;
    ROR() : Inst(Inst_ROR) {}
    static bool classof(const Inst *st) {
        return st->GetKind() == Inst_ROR;
    }
    void Accept(Visitor *v) override {
        return v->VisitROR(this);
    }     
    void Dump() override {
        llvm::outs() << hex(Addr(), 8) << ": ";
        Label().size() > 0 ? (llvm::outs() << Label() << ": \t") : llvm::outs() << "\t";
        llvm::outs() <<  "ROR \t" << "R" << reg1 << ", R" << reg2 << ", " << cx;
        if (ObjCode().size() > 0) {
            llvm::outs() << "\t\t=> " << ObjCode() << "\n";
        }else {
            llvm::outs() << "\n";
        }          
    }    
};

class SHL : public Inst{
public:
    int op{0x1E};
    int reg1;
    int reg2;
    int cx;
    SHL() : Inst(Inst_SHL) {}
    static bool classof(const Inst *st) {
        return st->GetKind() == Inst_SHL;
    }
    void Accept(Visitor *v) override {
        return v->VisitSHL(this);
    }  
    void Dump() override {
        llvm::outs() << hex(Addr(), 8) << ": ";
        Label().size() > 0 ? (llvm::outs() << Label() << ": \t") : llvm::outs() << "\t";
        llvm::outs() <<  "SHL \t" << "R" << reg1 << ", R" << reg2 << ", " << cx;
        if (ObjCode().size() > 0) {
            llvm::outs() << "\t\t=> " << ObjCode() << "\n";
        }else {
            llvm::outs() << "\n";
        }          
    }      
};

class SHR : public Inst{
public:
    int op{0x1F};
    int reg1;
    int reg2;
    int cx;
    SHR() : Inst(Inst_SHR) {}
    static bool classof(const Inst *st) {
        return st->GetKind() == Inst_SHR;
    }
    void Accept(Visitor *v) override {
        return v->VisitSHR(this);
    } 
    void Dump() override {
        llvm::outs() << hex(Addr(), 8) << ": ";
        Label().size() > 0 ? (llvm::outs() << Label() << ": \t") : llvm::outs() << "\t";
        llvm::outs() <<  "SHR \t" << "R" << reg1 << ", R" << reg2 << ", " << cx;
        if (ObjCode().size() > 0) {
            llvm::outs() << "\t\t=> " << ObjCode() << "\n";
        }else {
            llvm::outs() << "\n";
        }          
    }        
};

class JEQ : public Inst{
public:
    int op{0x20};
    llvm::StringRef cx;
    JEQ() : Inst(Inst_JEQ) {}
    static bool classof(const Inst *st) {
        return st->GetKind() == Inst_JEQ;
    }
    void Accept(Visitor *v) override {
        return v->VisitJEQ(this);
    }
    void Dump() override {
        llvm::outs() << hex(Addr(), 8) << ": ";
        Label().size() > 0 ? (llvm::outs() << Label() << ": \t") : llvm::outs() << "\t";
        llvm::outs() <<  "JEQ \t" << cx;
        if (ObjCode().size() > 0) {
            llvm::outs() << "\t\t=> " << ObjCode() << "\n";
        }else {
            llvm::outs() << "\n";
        }          
    }         
};

class JNE : public Inst{
public:
    int op{0x21};
    llvm::StringRef cx;
    JNE() : Inst(Inst_JNE) {}
    static bool classof(const Inst *st) {
        return st->GetKind() == Inst_JNE;
    }
    void Accept(Visitor *v) override {
        return v->VisitJNE(this);
    }  
    void Dump() override {
        llvm::outs() << hex(Addr(), 8) << ": ";
        Label().size() > 0 ? (llvm::outs() << Label() << ": \t") : llvm::outs() << "\t";
        llvm::outs() <<  "JNE \t" << cx;
        if (ObjCode().size() > 0) {
            llvm::outs() << "\t\t=> " << ObjCode() << "\n";
        }else {
            llvm::outs() << "\n";
        }          
    }               
};

class JLT : public Inst{
public:
    int op{0x22};
    llvm::StringRef cx;
    JLT() : Inst(Inst_JLT) {}
    static bool classof(const Inst *st) {
        return st->GetKind() == Inst_JLT;
    }
    void Accept(Visitor *v) override {
        return v->VisitJLT(this);
    }  
    void Dump() override {
        llvm::outs() << hex(Addr(), 8) << ": ";
        Label().size() > 0 ? (llvm::outs() << Label() << ": \t") : llvm::outs() << "\t";
        llvm::outs() <<  "JLT \t" << cx;
        if (ObjCode().size() > 0) {
            llvm::outs() << "\t\t=> " << ObjCode() << "\n";
        }else {
            llvm::outs() << "\n";
        }          
    }        
};

class JGT : public Inst{
public:
    int op{0x23};
    llvm::StringRef cx;
    JGT() : Inst(Inst_JGT) {}
    static bool classof(const Inst *st) {
        return st->GetKind() == Inst_JGT;
    }
    void Accept(Visitor *v) override {
        return v->VisitJGT(this);
    }   
    void Dump() override {
        llvm::outs() << hex(Addr(), 8) << ": ";
        Label().size() > 0 ? (llvm::outs() << Label() << ": \t") : llvm::outs() << "\t";
        llvm::outs() <<  "JGT \t" << cx;
        if (ObjCode().size() > 0) {
            llvm::outs() << "\t\t=> " << ObjCode() << "\n";
        }else {
            llvm::outs() << "\n";
        }          
    }       
};

class JLE : public Inst{
public:
    int op{0x24};
    llvm::StringRef cx;
    JLE() : Inst(Inst_JLE) {}
    static bool classof(const Inst *st) {
        return st->GetKind() == Inst_JLE;
    }
    void Accept(Visitor *v) override {
        return v->VisitJLE(this);
    }  
    void Dump() override {
        llvm::outs() << hex(Addr(), 8) << ": ";
        Label().size() > 0 ? (llvm::outs() << Label() << ": \t") : llvm::outs() << "\t";
        llvm::outs() <<  "JLE \t" << cx;
        if (ObjCode().size() > 0) {
            llvm::outs() << "\t\t=> " << ObjCode() << "\n";
        }else {
            llvm::outs() << "\n";
        }          
    }       
};

class JGE : public Inst{
public:
    int op{0x25};
    llvm::StringRef cx;
    JGE() : Inst(Inst_JGE) {}
    static bool classof(const Inst *st) {
        return st->GetKind() == Inst_JGE;
    }
    void Accept(Visitor *v) override {
        return v->VisitJGE(this);
    } 
    void Dump() override {
        llvm::outs() << hex(Addr(), 8) << ": ";
        Label().size() > 0 ? (llvm::outs() << Label() << ": \t") : llvm::outs() << "\t";
        llvm::outs() <<  "JGE \t" << cx;
        if (ObjCode().size() > 0) {
            llvm::outs() << "\t\t=> " << ObjCode() << "\n";
        }else {
            llvm::outs() << "\n";
        }          
    }         
};

class JMP : public Inst{
public:
    int op{0x26};
    llvm::StringRef cx;
    JMP() : Inst(Inst_JMP) {}
    static bool classof(const Inst *st) {
        return st->GetKind() == Inst_JMP;
    }
    void Accept(Visitor *v) override {
        return v->VisitJMP(this);
    }  
    void Dump() override {
        llvm::outs() << hex(Addr(), 8) << ": ";
        Label().size() > 0 ? (llvm::outs() << Label() << ": \t") : llvm::outs() << "\t";
        llvm::outs() <<  "JMP \t" << cx;
        if (ObjCode().size() > 0) {
            llvm::outs() << "\t\t=> " << ObjCode() << "\n";
        }else {
            llvm::outs() << "\n";
        }          
    }         
};

class SWI : public Inst{
public:
    int op{0x2A};
    int cx;
    SWI() : Inst(Inst_SWI) {}
    static bool classof(const Inst *st) {
        return st->GetKind() == Inst_SWI;
    }
    void Accept(Visitor *v) override {
        return v->VisitSWI(this);
    }  
    void Dump() override {
        llvm::outs() << hex(Addr(), 8) << ": ";
        Label().size() > 0 ? (llvm::outs() << Label() << ": \t") : llvm::outs() << "\t";
        llvm::outs() <<  "SWI \t" << cx;
        if (ObjCode().size() > 0) {
            llvm::outs() << "\t\t=> " << ObjCode() << "\n";
        }else {
            llvm::outs() << "\n";
        }          
    }            
};

class CALL : public Inst{
public:
    int op{0x2B};
    llvm::StringRef cx;
    CALL() : Inst(Inst_CALL) {}
    static bool classof(const Inst *st) {
        return st->GetKind() == Inst_CALL;
    }
    void Accept(Visitor *v) override {
        return v->VisitCALL(this);
    }  
    void Dump() override {
        llvm::outs() << hex(Addr(), 8) << ": ";
        Label().size() > 0 ? (llvm::outs() << Label() << ": \t") : llvm::outs() << "\t";
        llvm::outs() <<  "CALL \t" << cx;
        if (ObjCode().size() > 0) {
            llvm::outs() << "\t\t=> " << ObjCode() << "\n";
        }else {
            llvm::outs() << "\n";
        }          
    }           
};

class RET : public Inst{
public:
    int op{0x2C};
    RET() : Inst(Inst_RET) {}
    static bool classof(const Inst *st) {
        return st->GetKind() == Inst_RET;
    }
    void Accept(Visitor *v) override {
        return v->VisitRET(this);
    } 
    void Dump() override {
        llvm::outs() << hex(Addr(), 8) << ": ";
        Label().size() > 0 ? (llvm::outs() << Label() << ": \t") : llvm::outs() << "\t";
        llvm::outs() <<  "RET ";
        if (ObjCode().size() > 0) {
            llvm::outs() << "\t\t=> " << ObjCode() << "\n";
        }else {
            llvm::outs() << "\n";
        }          
    }    
};

class IRET : public Inst{
public:
    int op{0x2D};
    IRET() : Inst(Inst_IRET) {}
    static bool classof(const Inst *st) {
        return st->GetKind() == Inst_IRET;
    }
    void Accept(Visitor *v) override {
        return v->VisitIRET(this);
    }  
    void Dump() override {
        llvm::outs() << hex(Addr(), 8) << ": ";
        Label().size() > 0 ? (llvm::outs() << Label() << ": \t") : llvm::outs() << "\t";
        llvm::outs() <<  "IRET ";
        if (ObjCode().size() > 0) {
            llvm::outs() << "\t\t=> " << ObjCode() << "\n";
        }else {
            llvm::outs() << "\n";
        }           
    }        
};

class PUSH : public Inst{
public:
    int op{0x30};
    int reg1;
    PUSH() : Inst(Inst_PUSH) {}
    static bool classof(const Inst *st) {
        return st->GetKind() == Inst_PUSH;
    }
    void Accept(Visitor *v) override {
        return v->VisitPUSH(this);
    }  
    void Dump() override {
        llvm::outs() << hex(Addr(), 8) << ": ";
        Label().size() > 0 ? (llvm::outs() << Label() << ": \t") : llvm::outs() << "\t";
        llvm::outs() <<  "PUSH \t" << " R" << reg1;
        if (ObjCode().size() > 0) {
            llvm::outs() << "\t\t=> " << ObjCode() << "\n";
        }else {
            llvm::outs() << "\n";
        }           
    }      
};

class POP : public Inst{
public:
    int op{0x31};
    int reg1;
    POP() : Inst(Inst_POP) {}
    static bool classof(const Inst *st) {
        return st->GetKind() == Inst_POP;
    }
    void Accept(Visitor *v) override {
        return v->VisitPOP(this);
    }   
    void Dump() override {
        llvm::outs() << hex(Addr(), 8) << ": ";
        Label().size() > 0 ? (llvm::outs() << Label() << ": \t") : llvm::outs() << "\t";
        llvm::outs() <<  "POP \t" << " R" << reg1;
        if (ObjCode().size() > 0) {
            llvm::outs() << "\t\t=> " << ObjCode() << "\n";
        }else {
            llvm::outs() << "\n";
        }           
    }            
};

class PUSHB : public Inst{
public:
    int op{0x32};
    int reg1;
    PUSHB() : Inst(Inst_PUSHB) {}
    static bool classof(const Inst *st) {
        return st->GetKind() == Inst_PUSHB;
    }
    void Accept(Visitor *v) override {
        return v->VisitPUSHB(this);
    }  
    void Dump() override {
        llvm::outs() << hex(Addr(), 8) << ": ";
        Label().size() > 0 ? (llvm::outs() << Label() << ": \t") : llvm::outs() << "\t";
        llvm::outs() <<  "PUSHB \t" << " R" << reg1;
        if (ObjCode().size() > 0) {
            llvm::outs() << "\t\t=> " << ObjCode() << "\n";
        }else {
            llvm::outs() << "\n";
        }           
    }       
};

class POPB : public Inst{
public:
    int op{0x33};
    int reg1;
    POPB() : Inst(Inst_POPB) {}
    static bool classof(const Inst *st) {
        return st->GetKind() == Inst_POPB;
    }
    void Accept(Visitor *v) override {
        return v->VisitPOPB(this);
    }
    void Dump() override {
        llvm::outs() << hex(Addr(), 8) << ": ";
        Label().size() > 0 ? (llvm::outs() << Label() << ": \t") : llvm::outs() << "\t";
        llvm::outs() <<  "POPB \t" << " R" << reg1;
        if (ObjCode().size() > 0) {
            llvm::outs() << "\t\t=> " << ObjCode() << "\n";
        }else {
            llvm::outs() << "\n";
        }         
    }           
};

class RESW : public Inst{
public:
    int op{0xF0};
    int num;
    RESW() : Inst(Inst_RESW) {}
    static bool classof(const Inst *st) {
        return st->GetKind() == Inst_RESW;
    }
    void Accept(Visitor *v) override {
        return v->VisitRESW(this);
    } 
    void Dump() override {
        llvm::outs() << hex(Addr(), 8) << ": ";
        Label().size() > 0 ? (llvm::outs() << Label() << ": \t") : llvm::outs() << "\t";
        llvm::outs() <<  "RESW \t" << num;
        if (ObjCode().size() > 0) {
            llvm::outs() << "\t\t=> " << ObjCode() << "\n";
        }else {
            llvm::outs() << "\n";
        }        
    }          
};

class RESB : public Inst{
public:
    int op{0xF1};
    int num;
    RESB() : Inst(Inst_RESB) {}
    static bool classof(const Inst *st) {
        return st->GetKind() == Inst_RESB;
    }
    void Accept(Visitor *v) override {
        return v->VisitRESB(this);
    } 
    void Dump() override {
        llvm::outs() << hex(Addr(), 8) << ": ";
        Label().size() > 0 ? (llvm::outs() << Label() << ": \t") : llvm::outs() << "\t";
        llvm::outs() <<  "RESB \t" << num;

        if (ObjCode().size() > 0) {
            llvm::outs() << "\t\t=> " << ObjCode() << "\n";
        }else {
            llvm::outs() << "\n";
        }
    }                    
};

class WORD : public Inst{
public:
    int op{0xF2};
    struct symbol {
        llvm::StringRef s;
    };
    llvm::SmallVector<std::variant<symbol, int>> args;
    WORD() : Inst(Inst_WORD) {}
    static bool classof(const Inst *st) {
        return st->GetKind() == Inst_WORD;
    }
    void Accept(Visitor *v) override {
        return v->VisitWORD(this);
    }   
    void Dump() override {
        llvm::outs() << hex(Addr(), 8) << ": ";
        Label().size() > 0 ? (llvm::outs() << Label() << ": \t") : llvm::outs() << "\t";
        llvm::outs() <<  "WORD \t";
        for (int i = 0; i < args.size(); ++i) {
            std::visit([&](auto &&v) {
                using T = std::decay_t<decltype(v)>;
                if constexpr (std::is_same_v<symbol, T>) {
                    llvm::outs() << v.s;
                }else {
                    llvm::outs() << v;
                }
            }, args[i]);
            if (i != args.size() - 1) {
                llvm::outs() << ",";
            }
        }
        
        if (ObjCode().size() > 0) {
            llvm::outs() << "\t\t=> " << ObjCode() << "\n";
        }else {
            llvm::outs() << "\n";
        }
    }       
};

class BYTE : public Inst{
public:
    int op{0xF3};
    struct symbol {
        llvm::StringRef s;
    };
    llvm::SmallVector<std::variant<symbol, llvm::StringRef, int>> args;
    BYTE() : Inst(Inst_BYTE) {}
    static bool classof(const Inst *st) {
        return st->GetKind() == Inst_BYTE;
    }
    void Accept(Visitor *v) override {
        return v->VisitBYTE(this);
    }
    void Dump() override {
        llvm::outs() << hex(Addr(), 8) << ": ";
        Label().size() > 0 ? (llvm::outs() << Label() << ": \t") : llvm::outs() << "\t";
        llvm::outs() <<  "BYTE \t";
        for (int i = 0; i < args.size(); ++i) {
            std::visit([&](auto &&v) {
                using T = std::decay_t<decltype(v)>;
                if constexpr (std::is_same_v<symbol, T>) {
                    llvm::outs() << v.s;
                }else {
                    llvm::outs() << v;
                }
            }, args[i]);
            if (i != args.size() - 1) {
                llvm::outs() << ",";
            }
        }
        
        if (ObjCode().size() > 0) {
            llvm::outs() << "\t\t=> " << ObjCode() << "\n";
        }else {
            llvm::outs() << "\n";
        }
    }          
};
}