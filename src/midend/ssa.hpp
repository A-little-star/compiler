#include "xc.hpp"
#include <vector>

void BuildDomTree(func_ptr func);
void ComputeDF(func_ptr func);
void toSSA(prog_ptr prog);