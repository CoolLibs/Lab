
import random

def relative_pos(index_of_the_term,number_of_terms):
    return (index_of_the_term - 1) / (number_of_terms - 1)


def gradient_test_generated():
    out = f"""
#include "_ROOT_FOLDER_/res/shader-examples/gradient/Mark.glsl"

#if TEST_IDX == 0

Mark gradient_data[];

"""
    for i in range(1,51): 
        out += f"""
#elif TEST_IDX == {i}

Mark gradient_data[number_of_marks_test] = Mark[](

"""
        for j in range(1,i): # Go in this loop when i > 2
            out += f"""
Mark({relative_pos(j,i)}f, vec4({random.random()}f, {random.random()}f, {random.random()}f, 1.f)),

"""
        out += f"""
Mark({1}.f, vec4({random.random()}f, {random.random()}f, {random.random()}f, 1.f))
);

"""
    out += f"""
#endif

"""
    return out





if __name__ == '__main__':
    from tooling.generate_files import generate
    generate(
        folder="res/shader-examples/gradient/generated",
        files=[
            gradient_test_generated,
        ],
    )
