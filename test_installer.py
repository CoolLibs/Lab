import os


def main():
    from pathlib import Path

    # os.system(
    #     f"cmake --no-warn-unused-cli -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE:STRING=Release -SC:/Dev/Cool/Lab2 -Bc:/Dev/Cool/Lab2/build -G Ninja"
    # )
    # os.system(
    #     "cmake --build c:/Dev/Cool/Lab2/build --config Release --target Coollab --"
    # )
    os.chdir(Path(__file__).parent.joinpath("build"))
    os.system("cpack")
    os.system("Coollab-0.1.1-win64.exe")


if __name__ == "__main__":
    main()
