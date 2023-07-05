#include "xc.hpp"
#include <unordered_map>

extern std::unordered_map<value_ptr, int> val_map;

void basic_block::Dump(std::ostream &os) {
    os << "  " << name << ".next: ";
    for (size_t i = 0; i < next.size(); i ++ ) {
        os << next[i]->name;
        if (i < next.size() - 1) os << ", ";
    }
    os << std::endl;
    os << "    Def: ";
    for (const auto &element : def) {
        if (element->name != "") {
            os << element->name << "  ";
        }
        else {
            os << "%" << val_map[element] << "  ";
        }
    }
    os << std::endl;
    os << "    LiveUse: ";
    for (const auto &element : liveuse) {
        if (element->name != "") {
            os << element->name << "  ";
        }
        else {
            os << "%" << val_map[element] << "  ";
        }
    }
    os << std::endl;
    os << "    LiveIn: ";
    for (const auto &element : livein) {
        if (element->name != "") {
            os << element->name << "  ";
        }
        else {
            os << "%" << val_map[element] << "  ";
        }
    }
    os << std::endl;
    os << "    LiveOut: ";
    for (const auto &element : liveout) {
        if (element->name != "") {
            os << element->name << "  ";
        }
        else {
            os << "%" << val_map[element] << "  ";
        }
    }
    os << std::endl;
}

void value::Dump(std::ostream &os) {
    for (const auto &element : liveout) {
        if (element->name != "") os << element->name << "  ";
        else os << "%" << val_map[element] << "  ";
    }
    os << std::endl;
}

void DumpFlowGraph(const prog_ptr prog, std::ostream &os) {
    for (size_t i = 0; i < prog->funcs->len; i ++ ) {
        func_ptr func = (func_ptr)prog->funcs->buffer[i];
        os << func->name << ":\n";
        for (size_t j = 0; j < func->bbs->len; j ++ ) {
            basic_block_ptr bb = (basic_block_ptr)func->bbs->buffer[j];
            bb->Dump(os);
            for (size_t k = 0; k < bb->insts->len; k ++ ) {
                value_ptr val = (value_ptr)bb->insts->buffer[k];
                os << "      ";
                val->Dump(os);
            }
        }
    }
}