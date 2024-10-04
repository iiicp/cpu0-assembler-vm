#pragma once
#include <string>
#include "memory.h"
#include "llvm/ADT/SmallVector.h"
namespace cpu0 {
class VM {
public:
    explicit VM(llvm::StringRef objStr): objStr_(objStr) {
        m.Load(objStr);
        m.Dump();
    };
    void Run();
private:
    llvm::StringRef objStr_;
    Memory m;
    uint32_t IR{16};
    uint32_t PC{15};
    uint32_t LR{14};
    uint32_t SP{13};
    uint32_t SW{12};
    llvm::SmallVector<int64_t> R{0,1,2,3,4,5,6,7,8,9,10,11,0,13,-1,0,16};
};
}