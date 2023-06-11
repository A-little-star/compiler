#include "xc.hpp"
#include "AST.hpp"

program *genIR(BaseAST &ast)
{

}

void CompUnitAST::GenIR() const
{
    prog_ptr program = (prog_ptr)malloc(sizeof(program));
    func_def->GenIR();
}