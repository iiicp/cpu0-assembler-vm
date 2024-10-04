#include "vm.h"
namespace cpu0 {
void VM::Run() {
    volatile bool stop = false;
    while (!stop) {
        uint32_t tpc = R[PC];
        R[0] = 0;
        R[IR] = m.ReadIntBE(R[PC]);
        R[PC] += 4;
        int op = bits(R[IR], 24, 31);
        int ra = bits(R[IR], 20, 23);
        int rb = bits(R[IR], 16, 19);
        int rc = bits(R[IR], 12, 15);
        int c24 = signbits(R[IR], 0, 23);
        int c16 = signbits(R[IR], 0, 15);
        int c5 = signbits(R[IR], 0, 4);
        int addr = R[rb] + c16;
        int raddr = R[rb] + R[rc];
        int N = bits(R[SW], 31, 31);
        int Z = bits(R[SW], 30, 30);

        switch (op)
        {
        case 0x00:  // LD
            R[ra] = m.ReadIntBE(addr);
            break;
        case 0x01:  // ST
            m.WriteIntBE(addr, R[ra]);
            break;
        case 0x02:  // LDB
            R[ra] = m.ReadByte(addr);
            break;
        case 0x03:  // STB
            m.WriteByte(addr, R[ra]);
            break;
        case 0x04:  // LDR
            R[ra] = m.ReadIntBE(raddr);
            break;
        case 0x05:  // STR
            m.WriteIntBE(raddr, R[ra]);
            break;
        case 0x06:  // LBR
            R[ra] = m.ReadByte(raddr);
            break;     
        case 0x07:  // SBR
            m.WriteByte(raddr, R[ra]);
            break;
        case 0x08:  // LDI
            R[ra] = c16;
            break;
        case 0x10:  // CMP 
        {
            if (R[ra] > R[rb]) {                               // > : SW(N=0, Z=0)
                R[SW] &= 0x3FFFFFFF;                             // N=0, Z=0
            } else if (R[ra] < R[rb]) {                        // < : SW(N=1, Z=0, ....)                                                
                R[SW] |= 0x80000000;                             // N=1;
                R[SW] &= 0xBFFFFFFF;                             // Z=0;
            } else {                                           // = : SW(N=0, Z=1)                      
                R[SW] &= 0x7FFFFFFF;                             // N=0;
                R[SW] |= 0x40000000;                             // Z=1;
            }
            break;
        }
        case 0x12:  // MOV
            R[ra] = R[rb];
            break;
        case 0x13:  // ADD
            R[ra] = R[rb] + R[rc];
            break;
        case 0x14:  // SUB
            R[ra] = R[rb] - R[rc];
            break;
        case 0x15:  // MUL
            R[ra] = R[rb] * R[rc];
            break;  
        case 0x16:  // DIV
            R[ra] = R[rb] / R[rc];
            break;
        case 0x18:  // AND
            R[ra] = R[rb] & R[rc];
            break;
        case 0x19:  // OR
            R[ra] = R[rb] | R[rc];
            break;
        case 0x1A:  // XOR
            R[ra] = R[rb] ^ R[rc];
            break;
        case 0x1B:  // ADDI
            R[ra] = R[rb] + c16;
            break;
        case 0x1C:  // ROL
            break;
        case 0x1D:  // ROR
            break;
        case 0x1E:  // SHL
            R[ra] = R[rb] << c5;
            break;
        case 0x1F:  // SHR
            R[ra] = R[rb] >> c5;
            break;
        case 0x20:  // JEQ
            if (Z == 1) R[PC] += c24;
            break;
        case 0x21:  // JNE
            if (Z == 0) R[PC] += c24;
            break;    
        case 0x22:  // JLT
            if (N==1&&Z==0) R[PC] += c24;
            break;
        case 0x23:  // JGT
            if (N==0&&Z==0) R[PC] += c24;
            break;
        case 0x24:  // JLE
            if ((N==1&&Z==0)||(N==0&&Z==1)) R[PC]+=c24; 
            break;
        case 0x25:  // JGE
            if ((N==0&&Z==0)||(N==0&&Z==1)) R[PC]+=c24;
            break;  
        case 0x26:  // JMP
            R[PC]+=c24;
            break;
        case 0x2A:  // SWI
        {
            switch (c24) {
                case 3: printf("%s", m.ReadStr(R[9]).data()); break;
                case 4: printf("%d", R[9]); break;
                default: 
                    llvm::errs() << "SWI cx= " << c24 << "not found!\n"; 
                    break;
            }
            break;
        }
        case 0x2B:  // CALL
            R[LR] = R[PC];
            R[PC] += c24;
            break;
        case 0x2C:  // RET
            if (R[LR]<0) stop=true; else R[PC]=LR;
            break;      
        case 0x2D:  // IRET
            if (R[LR]<0) stop=true; else R[PC]=LR;
            break;
        case 0x30:  // PUSH
            R[SP]-=4; m.WriteIntBE(R[SP], R[ra]);
            break;   
        case 0x31:  // POP
            R[ra] = m.ReadIntBE(R[SP]); R[SP]+=4;
            break;
        case 0x32:  // PUSHB
            R[SP]--; m.WriteByte(R[SP], R[ra]);
            break;
        case 0x33:  // POPB
            R[ra] = m.ReadByte(R[SP]); R[SP]++;
            break;                                                                                                                                                                                                                       
        default:
            llvm::errs() << "not support " << op << "\n";
            break;
        }
    }
}
} 