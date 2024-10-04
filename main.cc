#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/ErrorOr.h"
#include "llvm/Support/raw_ostream.h"
#include "diag_engine.h"
#include "lexer.h"
#include "common.h"
#include "parser.h"
#include "as.h"
#include "vm.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("please input filename!\n");
        return 0;
    }

    const char *file_name = argv[1];

    static llvm::ErrorOr<std::unique_ptr<llvm::MemoryBuffer>> buf = llvm::MemoryBuffer::getFile(file_name);
    if (!buf) {
        llvm::errs() << "can't open file!!!\n";
        return -1;
    }

    llvm::SourceMgr mgr;
    DiagEngine diagEngine(mgr);

    mgr.AddNewSourceBuffer(std::move(*buf), llvm::SMLoc());

    cpu0::Lexer lexer(mgr, diagEngine);
    cpu0::Parser parser(lexer, diagEngine);
    const auto &insts = parser.Parse();
    llvm::outs() << "parser.... \n";
    for (const auto &inst : insts) {
        inst->Dump();
    }

    cpu0::AS as(insts, parser.SymbolTable(), diagEngine);
    const auto &hexObjStr = as.Assember();

    llvm::outs() << "assember.... \n";
    for (const auto &inst : insts) {
        inst->Dump();
    }

    llvm::outs() << "vm run.... \n";
    cpu0::VM vm(hexObjStr);
    vm.Run();

    // Token tok;
    // while (true) {
    //     lexer.NextToken(tok);
    //     if (tok.kind == TokenType::Eof)
    //         break;
    //     tok.Dump();
    // }
    return 0;
}