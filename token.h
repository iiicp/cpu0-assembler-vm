#pragma once
namespace cpu0 {
enum class TokenType : uint8_t {
    LD,     // L | 0x00 | load word     | LD Ra,[Rb+Cx]  | Ra = [Rb + Cx]
    ST,     // L | 0x01 | store word    | ST Ra,[Rb+Cx]  | [Rb+Cx] = Ra;
    LDB,    // L | 0x02 | load byte     | LDB Ra,[Rb+Cx] | Ra =(byte)[Rb+Cx]
    STB,    // L | 0x03 | store byte    | STB Ra,[Rb+Cx] | [Rb+Cx] = (byte)Ra
    LDR,    // A | 0x04 | LD的寄存器版本  | LDR Ra,[Rb+Rc] | Ra = [Rb+Rc]
    STR,    // A | 0x05 | ST的寄存器版本  | STR Ra,[Rb+Rc] | [Rb+Rc] = Ra
    LBR,    // A | 0x06 | LDB的寄存器版本 | LBR Ra,[Rb+Rc] | Ra = (byte)[Rb+Rc]
    SBR,    // A | 0x07 | SBR的寄存器版本 | SBR Ra,[Rb+Rc] | [Rb+Rc] = (byte)Ra
    LDI,    // L | 0x08 | 载入常数        | LDI Ra, Cx    | Ra = Cx
    CMP,    // A | 0x10 | 比较           | CMP Ra, Rb     | SW = Ra >=< Rb
    MOV,    // A | 0x12 | 移动           | MOV Ra, Rb     | Ra = Rb
    ADD,    // A | 0x13 | 加法           | ADD Ra, Rb, Rc | Ra = Rb + Rc
    SUB,    // A | 0x14 | 减法           | SUB Ra, Rb, Rc | Ra = Rb - Rc
    MUL,    // A | 0x15 | 乘法           | MUL Ra, Rb, Rc | Ra = Rb * Rc
    DIV,    // A | 0x16 | 除法           | DIV Ra, Rb, Rc | Ra = Rb / Rc
    AND,    // A | 0x18 | 按位与         | AND Ra, Rb, Rc | Ra = Rb & Rc
    OR,     // A | 0x19 | 按位或         | OR Ra, Rb, Rc  | Ra = Rb | Rc
    XOR,    // A | 0x1A | 按位异或        | XOR Ra,Rb,Rc  | Ra = Rb ^ Rc
    ADDI,   // A | 0x1B | 常数加法        | ADDI Ra,Rb,Cx | Ra = Rb+Cx
    ROL,    // A | 0x1C | 循环左移        | ROL Ra,Rb,Cx  | Ra = Rb rol Cx
    ROR,    // A | 0x1D | 循环右移        | ROR Ra,Rb,Cx  | Ra = Rb ror Cx
    SHL,    // A | 0x1E | 向左移位        | SHL Ra,Rb,Cx  | Ra = Rb << Cx
    SHR,    // A | 0x1F | 向右移位        | SHR Ra,Rb,Cx  | Ra = Rb >> Cx
    JEQ,    // J | 0x20 | 相等跳转        | JEQ Cx        | if SW(=) PC = PC+Cx
    JNE,    // J | 0x21 | 不等跳转        | JNE Cx        | if SW(!=) PC=PC+Cx
    JLT,    // J | 0x22 | 小于跳转        | JLT Cx        | if SW(<) PC= PC+Cx
    JGT,    // J | 0x23 | 大于跳转        | JGT Cx        | if SW(>) PC=PC+Cx
    JLE,    // J | 0x24 | 小于等于跳转     | JLE Cx        | if SW(<=) PC=PC+Cx
    JGE,    // J | 0x25 | 大于等于跳转     | JGE Cx        | if SW(>=) PC=PC+Cx
    JMP,    // J | 0x26 | 无条件跳转       | JMP Cx       | PC=PC+Cx
    SWI,    // J | 0x2A | 软件中断         | SWI Cx       | LR=PC; PC=Cx; INT=1
    CALL,   // J | 0x2B | 调用            | CALL Cx      | LR=PC; PC=PC+Cx
    RET,    // J | 0x2C | 返回            | RET          | PC=LR
    IRET,   // J | 0x2D | 中断返回         | IRET         | PC=LR; INT=0
    PUSH,   // A | 0x30 | push word       | PUSH Ra      | SP-=4;[SP]=Ra
    POP,    // A | 0x31 | pop word        | POP Ra       | Ra=[SP]; SP+=4
    PUSHB,  // A | 0x32 | push byte       | PUSH Ra      | SP--;[SP]=(byte)Ra
    POPB,   // A | 0x33 | pop byte        | POP Ra       | Ra=(byte)[SP]; SP++
    RESW,   // D | 0xF0 | 保留n个word      
    RESB,   // D | 0xF1 | 保留n个byte    
    WORD,   // D | 0xF2 | 定义有初值的整数变量
    BYTE,   // D | 0xf3 | 定义有初值的字节变量或者字符串
    COMMA,  // ,
    COLON,  // :
    Iden,  
    Num,
    String, 
    Eof
};

class Token {
public:
    Token(){}
    TokenType kind;
    int row, col;
    
    int numVal;
    std::string strVal;

    const char *ptr; // for debug && diag
    int len;

    void Dump() {
        llvm::outs() << "{" << llvm::StringRef(ptr, len) << ", row = " << row << ", col = " << col << "}\n";
    }
};
}