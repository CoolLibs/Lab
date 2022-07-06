#include <Cool/String/String.h>
#include <Cool/StrongTypes/RgbColor.h>

TEST_CASE("[Cool::String] replace_all()")
{
    std::string text = "Effect_intensity";
    Cool::String::replace_all(text, "_", " ");
    CHECK(text == "Effect intensity");
}

TEST_CASE("Find next parenthesized block")
{
    // CHECK(
    //     Cool::String::sdifhskf("default (1, 3) min (0 2)") ==
    //     "(1, 3)"
    // );

    // CHECK(
    //     Cool::String::sdifhskf("(ds, (dfg)) (dg, dft)") ==
    //     "(ds, (dfg))"
    // );
}

TEST_CASE("Parsing a float")
{
    CHECK(
        Cool::String::value_from_string<float>("2.f") ==
        2.f
    );
    CHECK(
        Cool::String::value_from_string<float>("2.") ==
        2.f
    );
    CHECK(
        Cool::String::value_from_string<float>("2") ==
        2.f
    );
    CHECK(
        Cool::String::value_from_string<float>("2.0") ==
        2.f
    );
    CHECK(
        Cool::String::value_from_string<float>("2.0f") ==
        2.f
    );
}

TEST_CASE("Parsing a vec2")
{
    CHECK(
        Cool::String::value_from_string<Cool::RgbColor>("(1, 3, 4)") ==
        Cool::RgbColor(glm::vec3(1.f, 3.f, 4.f))
    );
    CHECK(
        Cool::String::value_from_string<glm::vec2>("  ( 1   ,  3)   ") ==
        glm::vec2(1.f, 3.f)
    );
    CHECK(
        Cool::String::value_from_string<glm::vec2>("(1 3)") ==
        glm::vec2(1.f, 3.f)
    );
    CHECK(
        Cool::String::value_from_string<glm::vec2>("(1.f, 3.f)") ==
        glm::vec2(1.f, 3.f)
    );
    CHECK(
        Cool::String::value_from_string<glm::vec2>("(1., 3.)") ==
        glm::vec2(1.f, 3.f)
    );
    CHECK(
        Cool::String::value_from_string<glm::vec2>("(1., hello)") ==
        std::nullopt
    );
    CHECK(
        Cool::String::value_from_string<glm::vec2>("(hello, 1.)") ==
        std::nullopt
    );
    CHECK(
        Cool::String::value_from_string<glm::vec2>("(hello, world)") ==
        std::nullopt
    );
}