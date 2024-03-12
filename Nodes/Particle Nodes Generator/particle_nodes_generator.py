from __future__ import annotations
from dataclasses import dataclass
import hashlib
import shutil
import sys, os
import re
from typing import Any

# TODO(Particles) delete generated files that are not used anymore
# TODO(Particles) pk c'est parfois $ et parfois $$
# TODO(Particles) allow .clbtemp nodes to specify different names for 2D and 3D versions


@dataclass
class Replacement:
    Dim2: Any
    Dim3: Any


def variables_replacements():
    return {
        "vec": Replacement(
            Dim2="vec2",
            Dim3="vec3",
        ),
        "vec34": Replacement(
            Dim2="vec3",
            Dim3="vec4",
        ),
        "UV": Replacement(
            Dim2="UV",
            Dim3="vec3",
        ),
        "Point": Replacement(
            Dim2="Point2D",
            Dim3="vec3",
        ),
        "Direction": Replacement(
            Dim2="Direction2D",
            Dim3="vec3",
        ),
        "Particle": Replacement(
            Dim2="Particle2D",
            Dim3="Particle3D",
        ),
    }


def functions_replacements():
    return {
        "vec(a)": Replacement(
            Dim2=("vec2(a)", ""),
            Dim3=("vec3(a)", ""),
        ),
        "vec(a, b)": Replacement(
            Dim2=("vec2(a, b)", ""),
            Dim3=("vec3(a, b, 0)", ""),
        ),
        "vec(a, b, c)": Replacement(
            Dim2=("vec2(a, b)", ""),
            Dim3=("vec3(a, b, c)", ""),
        ),
        "vec34(a)": Replacement(
            Dim2=("vec3(a)", ""),
            Dim3=("vec4(a)", ""),
        ),
        "gradient(p, shape)": Replacement(
            Dim2=(
                "gradient(p)",
                """
vec2 gradient(vec2 p)
{
    float h = 0.001;
    return vec2(
        shape(p - vec2(h, 0)) - shape(p + vec2(h, 0)),
        shape(p - vec2(0, h)) - shape(p + vec2(0, h))
    ) / (2. * h);
}
    """,
            ),
            Dim3=(
                "gradient(p)",
                """
vec3 gradient(vec3 p)
{
    float h = 0.001;
    return vec3(
        shape(p - vec3(h, 0, 0)) - shape(p + vec3(h, 0, 0)),
        shape(p - vec3(0, h, 0)) - shape(p + vec3(0, h, 0)),
        shape(p - vec3(0, 0, h)) - shape(p + vec3(0, 0, h))
    ) / (2. * h);
}
""",
            ),
        ),
    }


def defines_2D():
    return ["2D"]


def defines_3D():
    return ["3D"]


###


@dataclass
class MacroVariable:
    name: str


@dataclass
class MacroFunction:
    name: str
    params: list[str | MacroVariable | MacroFunction]


@dataclass
class Macro:
    at: int
    len: int
    macro: MacroVariable | MacroFunction


def split_params(param_string: str):
    params: list[str] = []
    bracket_level = 0
    current_param = ""
    for char in param_string:
        if char == "(":
            bracket_level += 1
        elif char == ")":
            bracket_level -= 1
        elif char == "," and bracket_level == 0:
            params.append(current_param.strip())
            current_param = ""
            continue
        current_param = current_param + char
    if current_param:
        params.append(current_param.strip())
    return params


def parse_macro(macro_string: str):
    if macro_string[0] == "$":
        macro_string = macro_string[1:]
    if "(" not in macro_string:
        return MacroVariable(macro_string.strip())
    func_name, param_string = macro_string.split("(", 1)
    func_name = func_name.strip()
    param_string = param_string[:-1]
    params = split_params(param_string)
    return MacroFunction(func_name, [p.strip() for p in params])


def is_name_compatible(char: str):
    return char.isalnum() or char == "_"


def get_macro_string(line: str):
    bracket_level = 0
    is_name = True
    is_function = False
    i = 0
    for char in line:
        if not (is_name or is_function) and char != " ":
            i -= 1
            break
        if not (is_name_compatible(char) or char == "$"):
            is_name = False
        if not is_function and char == ")":
            break
        i += 1
        if char == "(":
            bracket_level += 1
            is_function = True
        if char == ")":
            bracket_level -= 1
            if bracket_level == 0:
                break
    return line[0:i]


def get_macros_alt(line: str):
    if "$" not in line:
        return line
    macros: list[MacroVariable | MacroFunction] = []
    i = 0
    while i < len(line):
        char = line[i]
        if char == "$":
            at = i
            macro_string = get_macro_string(line[i:])
            i += len(macro_string)
            macro = parse_macro(macro_string)
            if type(macro) is MacroVariable:
                macros.append(Macro(at, len(macro_string), macro))
                continue
            macro_params = []
            for param in macro.params:
                macro_params.append(get_macros_alt(param))
            macro.params = macro_params
            macros.append(Macro(at, len(macro_string), macro))
        else:
            i += 1
    return macros


def get_macros(line: str) -> list[Macro]:
    if "$" not in line:
        return []
    return get_macros_alt(line)


def print_macros(text: str):
    macros = get_macros(text)
    macro_i = 0
    for i in range(len(text)):
        try:
            if i == macros[macro_i].at:
                sys.stdout.write("\033[32m")
            if i == macros[macro_i].at + macros[macro_i].len:
                sys.stdout.write("\033[0m")
                macro_i += 1
        except:
            pass
        if text[i] == " ":
            sys.stdout.write("␣")
        else:
            sys.stdout.write(text[i])


def extract_snippets(text: str) -> list[str | MacroVariable | MacroFunction]:
    macros = get_macros(text)
    last_index = 0
    snippets = []
    for macro in macros:
        extract = text[last_index : macro.at]
        last_index = macro.at + macro.len
        snippets.append(extract)
        snippets.append(macro.macro)
    extract = text[last_index:]
    snippets.append(extract)
    return snippets


def extract_signature(inp: str) -> tuple[str, list[str]]:
    name, param_string = inp.split("(", 1)
    param_string = param_string[:-1]
    params = [p.strip() for p in param_string.split(",")]
    return (name, params)


def convert_function(macro: MacroFunction, inp: str, out: str, func: str):
    name_in, params_in = extract_signature(inp)
    name_out, params_out = extract_signature(out)
    params_output = []
    for i in range(len(macro.params)):
        param = macro.params[i]
        if params_in[i] not in params_out:
            func = re.sub(r"\b" + params_in[i] + r"\b", param, func)
            continue
        params_output.append(param)
    for i, p in enumerate(params_out):
        if len(params_output) == i:
            params_output.append(p)
    return f"{macro.name}({", ".join(params_output)})", name_out, func


def parse_snippets(
    snippets: list[str | MacroVariable | MacroFunction],
    variables: dict[str, str] = {},
    functions: dict[str, list[str]] = {},
    defines: list[str] = [],
):
    output = ""
    output_functions = ""
    f_names = [
        [extract_signature(f)[0], len(extract_signature(f)[1])] for f in functions
    ]
    used_signatures = {}
    define_stack = []
    is_off_define = False
    for snip in snippets:
        if type(snip) is str:
            if not is_off_define:
                output += snip
            continue
        if type(snip) is MacroVariable:
            if snip.name == "$else":
                is_off_define = not is_off_define
                continue
            elif snip.name == "$endif":
                if len(define_stack) == 0:
                    raise f"$$endif without $$if"
                define_stack.pop()
                if len(define_stack) == 0 or define_stack[-1] in defines:
                    is_off_define = False
                continue
            if snip.name in variables:
                output += variables[snip.name]
            else:
                output += snip.name
                raise Exception(f"{snip.name} is not a valid macro variable")
            continue
        if type(snip) is MacroFunction:
            if snip.name == "$if":
                define_stack.append(snip.params[0])
                if snip.params[0] not in defines:
                    is_off_define = True
            else:
                signature = f"{snip.name}({", ".join(snip.params)})"
                if signature in used_signatures:
                    call, name, function = used_signatures[signature]
                    output += call
                else:
                    snip_name = [snip.name, len(snip.params)]
                    try:
                        f_index = f_names.index(snip_name)
                        call, name, function = convert_function(
                            snip,
                            list(functions.keys())[f_index],
                            list(functions.values())[f_index][0],
                            list(functions.values())[f_index][1],
                        )
                        if len(function) > 0:
                            sig_hash = str(
                                int(
                                    hashlib.sha1(signature.encode("utf-8")).hexdigest(),
                                    16,
                                )
                                % (10**8)
                            )
                        else:
                            sig_hash = ""
                        call = re.sub(
                            r"\b" + snip.name + r"\b", f"{name}{sig_hash}", call
                        )
                        function = re.sub(
                            r"\b" + snip.name + r"\b", f"{name}{sig_hash}", function
                        )
                        output += call
                        output_functions += function
                        used_signatures[signature] = (call, name, function)
                    except:
                        raise Exception(f"{snip.name} is not a valid macro function")
    output = output_functions + "\n" + output
    return output


def get_variables_replacements(dimension: str):
    if dimension == "2D":
        return {key: value.Dim2 for key, value in variables_replacements().items()}
    if dimension == "3D":
        return {key: value.Dim3 for key, value in variables_replacements().items()}
    raise Exception(f"Invalid dimension: '{dimension}'")


def get_functions_replacements(dimension: str):
    if dimension == "2D":
        return {key: value.Dim2 for key, value in functions_replacements().items()}
    if dimension == "3D":
        return {key: value.Dim3 for key, value in functions_replacements().items()}
    raise Exception(f"Invalid dimension: '{dimension}'")


def get_defines(dimension: str):
    if dimension == "2D":
        return defines_2D()
    if dimension == "3D":
        return defines_3D()
    raise Exception(f"Invalid dimension: '{dimension}'")


def create_output_file_content(file: os.DirEntry[str], dimension: str):
    with open(file.path, "r") as f:
        content = f.read()

    output_file_content = parse_snippets(
        extract_snippets(content),
        get_variables_replacements(dimension),
        get_functions_replacements(dimension),
        get_defines(dimension),
    )

    clbtemp_file_path = os.path.relpath(
        file, os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    ).replace("\\", "/")
    return f"""\
/* -----------------------------------------------------------------------------
 * This file was automatically generated by a Python script.
 * PLEASE DON'T EDIT IT DIRECTLY, your changes would be overwritten the next time the script is run.
 * Instead, go to "{clbtemp_file_path}"
 * and see https://coollab-art.com/Tutorials/Writing%20Nodes/Particle%20nodes.
 * -----------------------------------------------------------------------------
 */
{output_file_content}\
"""


def create_files(dimension: str, category_number: str):
    input_folder = os.path.dirname(__file__)
    nodes_folder = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
    for input_dir in os.scandir(input_folder):
        if not input_dir.is_dir():
            continue
        split = input_dir.name.split(" ", 1)
        if len(split) == 1:
            split.append("")
        index, name = split
        output_dir_path = os.path.join(
            nodes_folder, f"{category_number}{index} Particle {dimension} {name}"
        )
        if not os.path.exists(output_dir_path):
            os.mkdir(output_dir_path)
        for file in os.scandir(input_dir.path):
            if not file.name.endswith(".clbtemp"):
                continue
            name = file.name.replace(".clbtemp", "")
            file_path = os.path.join(output_dir_path, f"{name}.clbnode")
            file_content = create_output_file_content(file, dimension)
            with open(file_path, "w") as output_file:
                output_file.write(file_content)


def main():
    create_files("2D", "8")
    create_files("3D", "9")


if __name__ == "__main__":
    main()
