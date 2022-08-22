#if TEST_IDX == 0

Mark gradient_data[];

#elif TEST_IDX == 1

Mark gradient_data[number_of_marks] = Mark[](
    Mark(0.15f, vec4(1, 0, 0, 1))
);

#elif TEST_IDX == 2

Mark gradient_data[number_of_marks] = Mark[](
    Mark(0.15f, vec4(.1, .5, .1, 1)),
    Mark(0.25f, vec4(0, 0, .1, 1))

);

#elif TEST_IDX == 3
Mark gradient_data[number_of_marks] = Mark[](
    Mark(0.15f, vec4(.1, .5, .1, 1)),
    Mark(0.25f, vec4(1, 0, 1, 1)),
    Mark(0.3f, vec4(0, .1, 1, 1))
);

#elif TEST_IDX == 4
Mark gradient_data[number_of_marks] = Mark[](
    Mark(0.15f, vec4(.1, .5, .1, 1)),
    Mark(0.25f, vec4(0, 0, 1, 1)),
    Mark(0.3f, vec4(0, .5, 1, 1)),
    Mark(0.51f, vec4(1, 0, .5, 1))
);
#elif TEST_IDX == 5
Mark gradient_data[number_of_marks] = Mark[](
    Mark(0.15f, vec4(.1, .5, .1, 1)),
    Mark(0.25f, vec4(0, 0, 1, 1)),
    Mark(0.35f, vec4(0, .1, 1, 1)),
    Mark(0.51f, vec4(1, 0, .5, 1)),
    Mark(0.55f, vec4(0.3, 1, 0, 1))
);
#elif TEST_IDX == 6
Mark gradient_data[number_of_marks] = Mark[](
    Mark(0.15f, vec4(.1, .5, .1, 1)),
    Mark(0.25f, vec4(0, 0, 1, 1)),
    Mark(0.3f, vec4(0, .1, 1, 1)),
    Mark(0.51f, vec4(1, 0, .5, 1)),
    Mark(0.55f, vec4(0.3, 1, 0, 1)),
    Mark(0.75f, vec4(0.5, 1, 0, 1))
);
#elif TEST_IDX == 7

Mark gradient_data[number_of_marks] = Mark[](
    Mark(0.15f, vec4(.1, .5, .1, 1)),
    Mark(0.25f, vec4(0, 0, 1, 1)),
    Mark(0.3f, vec4(0, .1, 1, 1)),
    Mark(0.51f, vec4(1, 0, .5, 1)),
    Mark(0.55f, vec4(0.3, 1, 0, 1)),
    Mark(0.75f, vec4(0.5, 1, 0, 1)),
    Mark(0.8f, vec4(1, 1, 0, 1))
);
#elif TEST_IDX == 8
Mark gradient_data[number_of_marks] = Mark[](
    Mark(0.15f, vec4(.1, .5, .1, 1)),
    Mark(0.25f, vec4(0, 0, 1, 1)),
    Mark(0.3f, vec4(0, .1, 1, 1)),
    Mark(0.51f, vec4(1, 0, .5, 1)),
    Mark(0.55f, vec4(0.3, 1, 0, 1)),
    Mark(0.75f, vec4(0.5, 1, 0, 1)),
    Mark(0.8f, vec4(1, 1, 0, 1)),
    Mark(0.85f, vec4(1, 0.25, 0.5, 1))
);
#elif TEST_IDX == 9

Mark gradient_data[number_of_marks] = Mark[](
    Mark(0.15f, vec4(.1, .5, .1, 1)),
    Mark(0.25f, vec4(0, 0, 1, 1)),
    Mark(0.3f, vec4(0, .1, 1, 1)),
    Mark(0.51f, vec4(1, 0, .5, 1)),
    Mark(0.55f, vec4(0.3, 0, 0, 1)),
    Mark(0.75f, vec4(0.5, 1, 0, 1)),
    Mark(0.8f, vec4(1, 1, 0, 1)),
    Mark(0.85f, vec4(1, 0.25, 0.5, 1)),
    Mark(0.95f, vec4(1, 0, 0.8, 1))
);

#endif
