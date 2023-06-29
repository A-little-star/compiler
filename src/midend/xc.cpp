#include <cstdio>
#include <iostream>
#include <unordered_map>
#include <assert.h>
#include "xc.hpp"

// 状态量，表示当前是否正在处理参数
static bool is_param;

// 函数声明
// IR生成部分
void GenCode(const prog_ptr prog, std::ostream &os);
void GenCode(const slice_ptr slice, std::ostream &os);
void GenCode(const func_ptr func, std::ostream &os);
void GenCode(const basic_block_ptr bb, std::ostream &os);
void GenCode(const value_ptr val, std::ostream &os);
// 释放内存空间部分
void FreeMem(slice_ptr slice);
void FreeMem(func_ptr func);
void FreeMem(basic_block_ptr bb);
void FreeMem(value_ptr val);

static int val_id = 0;
std::unordered_map<value_ptr, int> val_map;

void irDS2Text(const prog_ptr prog, std::ostream &os) {
    printf("The generation of IR code starts now.\n");
    GenCode(prog, os);
}

void GenCode(const prog_ptr prog, std::ostream &os) {
    os << "decl @getint(): i32\n";
    os << "decl @getch(): i32\n";
    os << "decl @getarray(*i32): i32\n";
    os << "decl @putint(i32)\n";
    os << "decl @putch(i32)\n";
    os << "decl @putarray(i32, *i32)\n";
    os << "decl @starttime()\n";
    os << "decl @stoptime()\n\n";

    GenCode(prog->values, os);
    os << std::endl;
    GenCode(prog->funcs, os);
}

void GenCode(const slice_ptr slice, std::ostream &os) {
    for (size_t i = 0; i < slice->len; i ++ ) {
        auto ptr = slice->buffer[i];
        switch (slice->kind) {
            case RISK_FUNCTION:
                GenCode((func_ptr)ptr, os);
                break;
            case RISK_BASIC_BLOCK:
                GenCode((basic_block_ptr)ptr, os);
                break;
            case RISK_VALUE:
                GenCode((value_ptr)ptr, os);
                break;
            default:
                assert(false);
        }
    }
}

void GenCode(const func_ptr func, std::ostream &os) {
    os << "fun " << func->name << "(";
    GenCode(func->params, os);
    os << ")";
    if (func->ty.tag == KOOPA_TYPE_INT32) os << ": i32";
    // else if (func->ty.tag == KOOPA_TYPE_UNIT) ;
    os << " {\n";
    GenCode(func->bbs, os);
    os << "}\n";
}

void GenCode(const basic_block_ptr bb, std::ostream &os) {
    os << bb->name << ":\n";
    GenCode(bb->insts, os);
}

void GenCode(const value_ptr val, std::ostream &os) {
    const auto &kind = val->kind;
    switch (kind.tag) {
        case IR_INTEGER:
        {
            os << kind.data.integer.value;
            break;
        }
        case IR_AGGREGATE:
        {
            os << "{";
            slice_ptr elems = val->kind.data.aggregate.elems;
            for (int i = 0; i < elems->len; i ++ ) {
                value_ptr num = (value_ptr)elems->buffer[i];
                // os << num->kind.data.integer.value;
                GenCode(num, os);
                if (i != elems->len - 1) os << ", ";
            }
            os << "}";
            break;
        }
        case IR_FUNC_ARG:
        {
            if (val->kind.data.func_arg.index > 1) os << ",";
            os << val->name << ": ";
            switch (val->ty.tag) {
                case KOOPA_TYPE_INT32: os << "i32"; break;
                default: assert(false);
            }
            break;
        }
        case IR_ALLOC:
        {
            os << "  " << val->name << " = alloc ";
            if (val->ty.tag == KOOPA_TYPE_INT32 ) {
                os << "i32" << std::endl;
            }
            else if (val->ty.tag == KOOPA_TYPE_ARRAY) {
                type_kind type = val->ty;
                while (type.tag == KOOPA_TYPE_ARRAY) {
                    os << "[";
                    if (type.data.array.base->tag == KOOPA_TYPE_POINTER) {
                        os << "i32, ";
                    }
                    os << type.data.array.len;
                    os << "]";
                    type = *type.data.array.base;
                }
                os << std::endl;
            }
            else {
                assert(false);
            }
            break;
        }
        case IR_GLOBAL_ALLOC:
        {
            if (val->ty.data.pointer.base->tag == KOOPA_TYPE_INT32) {
                os << "global " << val->name << " = alloc ";
                os << "i32";
                os << ", " << val->kind.data.global_alloc.init->kind.data.integer.value << std::endl;
            }
            // else if (val->ty.'data.pointer.base->tag == KOOPA_TYPE_ARRAY) {
            //     os << "global " << val->name << " = alloc ";
            //     type_kind type = *val->ty.data.pointer.base;
            //     while (type.tag == KOOPA_TYPE_ARRAY) {
            //         os << "[";
            //         if (type.data.array.base->tag == KOOPA_TYPE_INT32) {
            //             os << "i32, " << type.data.array.len;
            //         }
            //         os << "]";
            //         type = *type.data.array.base;
            //     }
            //     os << ", ";
            //     GenCode(val->kind.data.global_alloc.init, os);
            //     os << std::endl;
            // }
            else if (val->ty.tag == KOOPA_TYPE_ARRAY) {
                os << "global " << val->name << " = alloc ";
                type_kind type = val->ty;
                while (type.tag == KOOPA_TYPE_ARRAY) {
                    os << "[";
                    if (type.data.array.base->tag == KOOPA_TYPE_POINTER) {
                        os << "i32, ";
                    }
                    os << type.data.array.len;
                    os << "]";
                    type = *type.data.array.base;
                }
                os << ", ";
                GenCode(val->kind.data.global_alloc.init, os);
                os << std::endl;
            }
            break;
        }
        case IR_LOAD:
        {
            printf("Here is load!\n");
            if (kind.data.load.src->kind.tag == IR_GET_ELEM_PTR) {
                os << "  %" << val_id << " = load " << "%" << val_map[kind.data.load.src] << std::endl;
                val_map[val] = val_id;
                val_id ++;
                break;
            }
            os << "  %" << val_id << " = load " << kind.data.load.src->name << std::endl;
            val_map[val] = val_id;
            val_id ++;
            break;
        }
        case IR_STORE:
        {
            printf("Here is IR_STORE!\n");
            os << "  store ";

            switch (kind.data.store.value->kind.tag) {
                case IR_INTEGER: os << kind.data.store.value->kind.data.integer.value << ", "; break;
                case IR_CALL:
                case IR_GET_ELEM_PTR:
                case IR_BINARY:
                case IR_LOAD: os << "%" << val_map[kind.data.store.value] << ", "; break;
                case IR_FUNC_ARG: os << kind.data.store.value->name << ", "; break;
                default: assert(false);
            }

            if (kind.data.store.dest->kind.tag == IR_GET_ELEM_PTR) os << "%" << val_map[kind.data.store.dest] << std::endl;

            // os << kind.data.store.dest->kind.data.alloc.name << std::endl;
            else os << kind.data.store.dest->name << std::endl;
            printf("IR_STORE is OK!\n");
            break;
        }
        case IR_GET_ELEM_PTR:
        {
            if (val->kind.data.get_elem_ptr.index->kind.tag == IR_INTEGER) {
                if (val->kind.data.get_elem_ptr.src->kind.tag == IR_ALLOC || val->kind.data.get_elem_ptr.src->kind.tag == IR_GLOBAL_ALLOC)
                    os << "  %" << val_id << " = getelemptr " << val->kind.data.get_elem_ptr.src->name << ", " << val->kind.data.get_elem_ptr.index->kind.data.integer.value << std::endl;
                else if (val->kind.data.get_elem_ptr.src->kind.tag == IR_GET_ELEM_PTR) {
                    os << "  %" << val_id << " = getelemptr " << "%" << val_map[val->kind.data.get_elem_ptr.src] << ", " << val->kind.data.get_elem_ptr.index->kind.data.integer.value << std::endl;
                }
                else assert(false);
            }
            else {
                assert(false);
            }
            val_map[val] = val_id;
            val_id ++;
            break;
        }
        case IR_BRANCH:
        {
            
            os << "  br ";
            switch (kind.data.branch.cond->kind.tag) {
                case IR_INTEGER: os << std::to_string(kind.data.branch.cond->kind.data.integer.value); break;
                case IR_CALL:
                case IR_BINARY:
                case IR_GET_ELEM_PTR:
                case IR_LOAD: os << "%" << std::to_string(val_map[kind.data.branch.cond]); break;
                case IR_FUNC_ARG: os << kind.data.branch.cond->name; break;
                default: assert(false);
            }
            os << ", " << kind.data.branch.true_bb->name << ", " << kind.data.branch.false_bb->name << std::endl;
            
            break;
        }
        case IR_JUMP:
        {
            os << "  jump " << kind.data.jump.target->name << std::endl;
            break;
        }
        case IR_CALL:
        {
            is_param = true;
            if (val->ty.tag == KOOPA_TYPE_UNIT) {
                os << "  call " << val->kind.data.call.callee->name;
                os << "(";
                for (size_t i = 0; i < val->kind.data.call.args->len; i ++ ) {
                    if (i > 0) os << ", ";
                    value_ptr ptr = (value_ptr)val->kind.data.call.args->buffer[i];
                    switch (ptr->kind.tag) {
                        case IR_INTEGER: GenCode(ptr, os); break;
                        case IR_CALL:
                        case IR_BINARY:
                        case IR_GET_ELEM_PTR:
                        case IR_LOAD: os << "%" << val_map[ptr]; break;
                        case IR_FUNC_ARG: os << ptr->name; break;
                        default: assert(false);
                    }
                }
                os << ")" << std::endl;
            }
            else if (val->ty.tag == KOOPA_TYPE_INT32) {
                os << "  %" << std::to_string(val_id) << " = call " << val->kind.data.call.callee->name;
                val_map[val] = val_id;
                val_id ++;
                os << "(";
                // GenCode(val->kind.data.call.args, os);
                for (size_t i = 0; i < val->kind.data.call.args->len; i ++ ) {
                    if (i > 0) os << ", ";
                    value_ptr ptr = (value_ptr)val->kind.data.call.args->buffer[i];
                    switch (ptr->kind.tag) {
                        case IR_INTEGER: GenCode(ptr, os); break;
                        case IR_CALL:
                        case IR_BINARY:
                        case IR_GET_ELEM_PTR:
                        case IR_LOAD: os << "%" << val_map[ptr]; break;
                        case IR_FUNC_ARG: os << ptr->name; break;
                        default: assert(false);
                    }
                }
                os << ")" << std::endl;
            }
            is_param = false;
            break;
        }
        case IR_RETURN:
        {
            printf("Here is IR_RETURN!\n");
            if (kind.data.ret.value == NULL) {
                os << "  ret\n";
                break;
            }
            switch (kind.data.ret.value->kind.tag) {
                case IR_INTEGER: os << "  ret " << kind.data.ret.value->kind.data.integer.value << std::endl; break;
                case IR_CALL:
                case IR_LOAD:
                case IR_GET_ELEM_PTR:
                case IR_BINARY: os << "  ret %" << val_map[kind.data.ret.value] << std::endl; break;
                case IR_FUNC_ARG: os << kind.data.ret.value->name << std::endl; break;
                default: assert(false);
            }
            printf("IR_RETURN is OK!\n");
            break;
        }
        case IR_BINARY:
        {
            std::string l_str, r_str, op;
            // 生成第一个操作数的字符串
            switch (kind.data.binary.lhs->kind.tag) {
                case IR_INTEGER: l_str = std::to_string(kind.data.binary.lhs->kind.data.integer.value); break;
                case IR_CALL:
                case IR_BINARY:
                case IR_GET_ELEM_PTR:
                case IR_LOAD: l_str = "%" + std::to_string(val_map[kind.data.binary.lhs]); break;
                case IR_FUNC_ARG: l_str = kind.data.binary.lhs->name; break;
                default: assert(false);
            }
            // 生成第二个操作数的字符串
            switch (kind.data.binary.rhs->kind.tag) {
                case IR_INTEGER: r_str = std::to_string(kind.data.binary.rhs->kind.data.integer.value); break;
                case IR_CALL:
                case IR_BINARY:
                case IR_GET_ELEM_PTR:
                case IR_LOAD: r_str = "%" + std::to_string(val_map[kind.data.binary.rhs]); break;
                case IR_FUNC_ARG: r_str = kind.data.binary.rhs->name; break;
                default: assert(false);
            }
            // 生成操作符的字符串
            switch (kind.data.binary.op) {
                case NOT_EQ : op = "ne"; break;
                case EQ : op = "eq"; break;
                case GT : op = "gt"; break;
                case LT : op = "lt"; break;
                case GE : op = "ge"; break;
                case LE : op = "le"; break;
                case ADD : op = "add"; break;
                case SUB : op = "sub"; break;
                case MUL : op = "mul"; break;
                case DIV : op = "div"; break;
                case MOD : op = "mod"; break;
                case AND : op = "and"; break;
                case OR : op = "or"; break;
                case XOR : op = "xor"; break;
                case SHL : op = "shl"; break;
                case SHR : op = "shr"; break;
                case SAR : op = "sar"; break;
            }
            os << "  %" << std::to_string(val_id) << " = " << op << " " << l_str << ", " << r_str << std::endl;
            val_map[val] = val_id;
            val_id ++;
            break;
        }
        default: {
            printf("There is a invalid type in GenCode of value_ptr!\n");
            assert(false);
        }
    }
}


void FreeMem(prog_ptr prog) {
    FreeMem(prog->values);
    FreeMem(prog->funcs);
    delete prog;
    prog = NULL;
}

void FreeMem(slice_ptr slice) {
    for (size_t i = 0; i < slice->len; i ++ ) {
        auto ptr = slice->buffer[i];
        switch (slice->kind) {
            case RISK_FUNCTION:
                FreeMem((func_ptr)ptr);
                break;
            case RISK_BASIC_BLOCK:
                FreeMem((basic_block_ptr)ptr);
                break;
            case RISK_VALUE:
                FreeMem((value_ptr)ptr);
                break;
            default:
                assert(false);
        }
    }
    delete slice;
    slice = NULL;
}

void FreeMem(func_ptr func) {
    FreeMem(func->params);
    FreeMem(func->bbs);
    delete func;
    func = NULL;
}

void FreeMem(basic_block_ptr bb) {
    FreeMem(bb->used_by);
    FreeMem(bb->params);
    FreeMem(bb->insts);
    delete bb;
    bb = NULL;
}

void FreeMem(value_ptr val) {
    auto &kind = val->kind;
    switch (kind.tag) {
        case IR_CALL:
        {
            if (kind.data.call.args != NULL) {
                for (size_t i = 0; i < kind.data.call.args->len; i ++ ) {
                    value_ptr ptr = (value_ptr)kind.data.call.args->buffer[i];
                    if (ptr->kind.tag == IR_INTEGER) FreeMem(ptr);
                }
                // FreeMem(kind.data.call.args);
            }
            break;
        }
        case IR_RETURN:
        {
            if (kind.data.ret.value == NULL) break;
            if (kind.data.ret.value->kind.tag == IR_INTEGER)
                FreeMem(kind.data.ret.value);
            break;
        }
        case IR_BINARY:
        {
            if (kind.data.binary.lhs->kind.tag == IR_INTEGER)
                FreeMem(kind.data.binary.lhs);
            if (kind.data.binary.rhs->kind.tag == IR_INTEGER)
                FreeMem(kind.data.binary.rhs);
            break;
        }
        default:
            break;
    }
    delete val;
    val = NULL;
}