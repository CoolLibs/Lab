#pragma once
#include "CodeGenContext.h"
#include "Function.h"
#include "FunctionSignature.h"

namespace Lab {

auto gen_default_function(FunctionSignature, CodeGenContext&)
    -> ExpectedFunctionName;

} // namespace Lab
