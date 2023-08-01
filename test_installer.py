import os


def main():
    from pathlib import Path

    os.chdir(Path(__file__).parent.joinpath("build"))
    os.system("cpack")
    os.system('"Coollab-beta-8 WIP-Windows.exe"')


if __name__ == "__main__":
    main()
