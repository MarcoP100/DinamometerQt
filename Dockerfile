FROM arm32v7/debian:buster

# Install necessary packages
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    gcc-arm-linux-gnueabihf \
    g++-arm-linux-gnueabihf \
    qtbase5-dev \
    qttools5-dev-tools \
    qt5-qmake \
    libqt5svg5-dev

# Set the working directory
WORKDIR /project
