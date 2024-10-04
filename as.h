#pragma once
#include "inst.h"
#include "diag_engine.h"
namespace cpu0 {
class AS : public Visitor{
public:
    AS(const llvm::SmallVector<std::shared_ptr<Inst>> &instVec, const llvm::DenseMap<llvm::StringRef, std::shared_ptr<Inst>> &symbolTable, DiagEngine &diagEngine)
    :instVec_(instVec), symbolTable_(symbolTable), diagEngine(diagEngine) {
        
    };

    const std::string & Assember();

private:   
    void VisitLD(LD *t) override;
    void VisitST(ST *t) override;
    void VisitLDB(LDB *t) override;
    void VisitSTB(STB *t) override;
    void VisitLDR(LDR *t) override;
    void VisitSTR(STR *t) override;
    void VisitLBR(LBR *t) override;
    void VisitSBR(SBR *t) override;
    void VisitLDI(LDI *t) override;
    void VisitCMP(CMP *t) override;
    void VisitMOV(MOV *t) override;
    void VisitADD(ADD *t) override;
    void VisitSUB(SUB *t) override;
    void VisitMUL(MUL *t) override;
    void VisitDIV(DIV *t) override;
    void VisitAND(AND *t) override;
    void VisitOR(OR *t) override;
    void VisitXOR(XOR *t) override;
    void VisitADDI(ADDI *t) override;
    void VisitROL(ROL *t) override;
    void VisitROR(ROR *t) override;
    void VisitSHL(SHL *t) override;
    void VisitSHR(SHR *t) override;
    void VisitJEQ(JEQ *t) override;
    void VisitJNE(JNE *t) override;
    void VisitJLT(JLT *t) override;
    void VisitJGT(JGT *t) override;
    void VisitJLE(JLE *t) override;
    void VisitJGE(JGE *t) override;
    void VisitJMP(JMP *t) override;
    void VisitSWI(SWI *t) override;
    void VisitCALL(CALL *t) override;
    void VisitRET(RET *t) override;
    void VisitIRET(IRET *t) override;
    void VisitPUSH(PUSH *t) override;
    void VisitPOP(POP *t) override;
    void VisitPUSHB(PUSHB *t) override;
    void VisitPOPB(POPB *t) override;
    void VisitRESW(RESW *t) override;
    void VisitRESB(RESB *t) override;
    void VisitWORD(WORD *t) override;
    void VisitBYTE(BYTE *t) override;

private:
    const llvm::SmallVector<std::shared_ptr<Inst>> &instVec_;
    const llvm::DenseMap<llvm::StringRef, std::shared_ptr<Inst>> &symbolTable_;
    DiagEngine &diagEngine;
    std::string out_;
    uint32_t ra{0};
    uint32_t rb{0};
    uint32_t rc{0};
    uint32_t cx{0};
    uint32_t pc{0};
};
}