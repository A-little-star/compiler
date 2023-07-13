#include "xc.hpp"
#include <vector>
#include <assert.h>

void BuildDomTree(func_ptr func);
void ComputeDF(func_ptr func);
void toSSA(prog_ptr prog);