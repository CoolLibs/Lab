from __future__ import annotations
from dataclasses import dataclass
import hashlib
import shutil
import sys, os
import re


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
    params = [p.strip() for p in params]
    return MacroFunction(func_name, params)


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
        return
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
                print(f"WARNING: {snip.name} is not a valid macro variable")
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
                        print(f"WARNING: {snip.name} is not a valid macro function")
    output = output_functions + "\n" + output
    return output


VARIABLES_2D = {
    "vec": "vec2",
    "vec23": "vec2",
    "vec34": "vec3",
    "UV": "UV",
    "Point2D": "Point2D",
    "Direction": "Direction2D",
    "Particle": "Particle2D",
}
FUNCTIONS_2D = {
    "vec(a)": ("vec2(a)", ""),
    "vec(a, b)": ("vec2(a, b)", ""),
    "vec(a, b, c)": ("vec2(a, b)", ""),
    "vec(a, b, c, d)": ("vec2(a, b)", ""),
    "vec23(a)": ("vec2(a)", ""),
    "vec23(a, b, c)": ("vec2(a, b)", ""),
    "vec34(a)": ("vec3(a)", ""),
    "vec34(a, b, c, d)": ("vec3(a, b, c)", ""),
    "gradient(p, shape)": (
        "gradient(p)",
        """
vec2 gradient(vec2 p)
{
    float h = 0.001;
    return vec2(
               shape(p - vec2(h, 0)) - shape(p + vec2(h, 0)),
               shape(p - vec2(0, h)) - shape(p + vec2(0, h))
           )
           / (2. * h);
}
""",
    ),
}
DEFINES_2D = ["IS_2D"]

VARIABLES_3D = {
    "vec": "vec3",
    "vec23": "vec3",
    "vec34": "vec4",
    "UV": "vec3",
    "Point2D": "vec3",
    "Direction": "vec3",
    "Particle": "Particle3D",
}
FUNCTIONS_3D = {
    "vec(a)": ("vec3(a)", ""),
    "vec(a, b)": ("vec3(a, b, 0)", ""),
    "vec(a, b, c)": ("vec3(a, b, c)", ""),
    "vec(a, b, c, d)": ("vec3(a, b, c)", ""),
    "vec23(a)": ("vec3(a)", ""),
    "vec23(a, b, c)": ("vec3(a, b, c)", ""),
    "vec34(a)": ("vec4(a)", ""),
    "vec34(a, b, c, d)": ("vec4(a, b, c, d)", ""),
    "gradient(p, shape)": (
        "gradient(p)",
        """
vec3 gradient(vec3 p)
{
    float h = 0.001;
    return vec3(
               shape(p - vec3(h, 0, 0)) - shape(p + vec3(h, 0, 0)),
               shape(p - vec3(0, h, 0)) - shape(p + vec3(0, h, 0)),
               shape(p - vec3(0, 0, h)) - shape(p + vec3(0, 0, h))
           )
           / (2. * h);
}
""",
    ),
}
DEFINES_3D = ["IS_3D"]


def read_and_parse(file: os.DirEntry[str]):
    f_in = open(file.path)
    content = f_in.read()
    PARSED_2D = parse_snippets(
        extract_snippets(content), VARIABLES_2D, FUNCTIONS_2D, DEFINES_2D
    )
    PARSED_3D = parse_snippets(
        extract_snippets(content), VARIABLES_3D, FUNCTIONS_3D, DEFINES_3D
    )
    WARNING = f"""\
/* -----------------------------------------------------------------------------
 * This file was automatically generated by a Python script.
 * PLEASE DON'T EDIT IT DIRECTLY, your changes would be overwritten the next time the script is run.
 * Instead, go to "input/{file.name}" and see https://coollab-art.com/Tutorials/Writing%20Nodes/Particle%20nodes.
 * -----------------------------------------------------------------------------
 */\n\
"""
    PARSED_2D = WARNING + PARSED_2D
    PARSED_3D = WARNING + PARSED_3D
    return PARSED_2D, PARSED_3D


INPUT = os.path.join(os.path.dirname(__file__), "input")
OUTPUT = os.path.join(os.path.dirname(__file__), "output")
NODES_FOLDER = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))

if not os.path.exists(OUTPUT):
    os.mkdir(OUTPUT)

for dir in os.scandir(INPUT):
    if not dir.is_dir():
        continue
    split = dir.name.split(" ", 1)
    if len(split) == 1:
        split.append("")
    index, name = split
    DIRNAME_2D = f"8{index} Particle 2D {name}".strip()
    DIRNAME_3D = f"9{index} Particle 3D {name}".strip()
    PATH_2D = os.path.join(OUTPUT, DIRNAME_2D)
    PATH_3D = os.path.join(OUTPUT, DIRNAME_3D)
    if not os.path.exists(PATH_2D):
        os.mkdir(PATH_2D)
    if not os.path.exists(PATH_3D):
        os.mkdir(PATH_3D)
    for file in os.scandir(dir.path):
        if not file.name.endswith(".clbtemp"):
            continue
        name = file.name.replace(".clbtemp", "")
        FILE_2D = os.path.join(PATH_2D, f"{name} 2D.clbnode")
        FILE_3D = os.path.join(PATH_3D, f"{name} 3D.clbnode")
        PARSED_2D, PARSED_3D = read_and_parse(file)
        f_2d = open(FILE_2D, "w+")
        f_3d = open(FILE_3D, "w+")
        f_2d.write(PARSED_2D)
        f_3d.write(PARSED_3D)
        f_2d.close()
        f_3d.close()

if os.path.split(NODES_FOLDER)[-1] != "Nodes":
    print(
        "Stopping there because this script's directory is not in the Nodes directory"
    )
    exit()

for dir in os.scandir(OUTPUT):
    destination = os.path.join(NODES_FOLDER, dir.name)
    if not os.path.exists(destination):
        os.mkdir(destination)
    for file in os.scandir(dir.path):
        shutil.move(file.path, os.path.join(destination, file.name))
    os.rmdir(dir.path)
os.rmdir(OUTPUT)
