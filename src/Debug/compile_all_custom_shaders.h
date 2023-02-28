#pragma once

#include <Cool/Dependencies/Input.h>
#include <Cool/Dependencies/InputFactory_Ref.h>
#include <Cool/Dependencies/InputProvider_Ref.h>
#include "Dependencies/UpdateContext_Ref.h"

namespace Lab {

void compile_all_custom_shaders(
    Cool::InputProvider_Ref,
    Cool::InputFactory_Ref,
    Cool::InputDestructor_Ref,
    UpdateContext_Ref
);

} // namespace Lab
