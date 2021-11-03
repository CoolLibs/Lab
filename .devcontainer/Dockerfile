# See here for image contents: https://github.com/microsoft/vscode-dev-containers/tree/v0.194.3/containers/cpp/.devcontainer/base.Dockerfile

# [Choice] Debian / Ubuntu version: debian-11, debian-10, debian-9, ubuntu-20.04, ubuntu-18.04
ARG VARIANT="buster"
FROM mcr.microsoft.com/vscode/devcontainers/cpp:0-${VARIANT}

# [Optional] Uncomment this section to install additional packages.
RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install --no-install-recommends \
        mesa-common-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev pkg-config cmake-data libgtk-3-dev \
        clang-format clang-tidy cppcheck flawfinder ninja-build python3-pip \
    && pip install lizard
