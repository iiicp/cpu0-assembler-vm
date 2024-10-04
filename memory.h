#pragma once
#include <vector>
#include <stdint.h>
#include <cassert>
#include <string>
#include <cstring>
#include <stdio.h>
#include "common.h"
#include "llvm/Support/raw_ostream.h"
namespace cpu0 {
class Memory {
private:
    std::vector<uint8_t> data_;
public:
    Memory(){}

    void WriteByte(size_t i, uint8_t b) {
        assert(i < data_.size());
        data_[i] = b;
    }

    uint8_t ReadByte(size_t i) {
        assert(i < data_.size());
        return data_[i];
    }

    void WriteIntBE(size_t i, int32_t w) {
        assert(i + 4 <= data_.size());
        data_[i] = static_cast<uint8_t>((w>>24) & 0xFF);
        data_[i+1] = static_cast<uint8_t>((w>>16) & 0xFF);
        data_[i+2] = static_cast<uint8_t>((w>>8) & 0xFF);
        data_[i+3] = static_cast<uint8_t>(w & 0xFF);
    }

    int32_t ReadIntBE(size_t i) {
        assert(i + 4 <= data_.size());
        int32_t value = (static_cast<int32_t>(data_[i]) << 24) |
                        (static_cast<int32_t>(data_[i+1]) << 16) |
                        (static_cast<int32_t>(data_[i+2]) << 8) |
                        (static_cast<int32_t>(data_[i+3]));
        return value;
    }

    void WriteUInt32BE(size_t i, uint32_t w) {
        assert(i + 4 <= data_.size());
        data_[i] = static_cast<uint8_t>((w>>24) & 0xFF);
        data_[i+1] = static_cast<uint8_t>((w>>16) & 0xFF);
        data_[i+2] = static_cast<uint8_t>((w>>8) & 0xFF);
        data_[i+3] = static_cast<uint8_t>(w & 0xFF);
    }

    int32_t ReadUIntBE(size_t i) {
        assert(i < data_.size());
        int32_t value = (static_cast<int32_t>(data_[i]) << 24) |
                        (static_cast<int32_t>(data_[i+1]) << 16) |
                        (static_cast<int32_t>(data_[i+2]) << 8) |
                        (static_cast<int32_t>(data_[i+3]));
        return value;
    }

    void WriteStr(size_t i, std::string str) {
        size_t len = str.size();
        assert(i + len < data_.size());
        memcpy(&data_[i], str.data(), len);
        data_[i+len] = '\0';
    }

    std::string ReadStr(size_t i) {
        std::string str;
        for (size_t k = i; k < data_.size(); k++) {
            if (data_[k] == '\0') {
                break;
            }else {
                str += data_[k];
            }
        }
        return str;
    }

    void Dump() {
        llvm::outs() << "memory: ";
        std::string dstr;
        for (size_t i = 0; i < data_.size(); ++i) {
            if ((i % 16) == 0) {
                dstr += "\n" + hex(i, 2) + " : ";
            }
            if ((i % 4) == 0) {
                dstr += " ";
            }
            dstr += hex(ReadByte(i), 2);
        }
        llvm::outs() << dstr << "\n";
    }

    void Load(llvm::StringRef objStr) {
        data_.resize(objStr.size()/2);
        for (int i = 0; i < objStr.size(); i+=2) {
            auto h2 = objStr.substr(i, 2);
            std::string h(h2.data(), h2.size());
            uint8_t b = strtol(h.data(), nullptr, 16);
            data_[i/2] = b;
        }
    }
};
}