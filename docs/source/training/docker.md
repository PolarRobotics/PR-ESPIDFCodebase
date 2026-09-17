# Docker

## Overview

- In this tutorial, you will install Docker which is a containerization platform that allows you to run applications in isolated environments called containers. It is used in our development environment to ensure consistency across different machines and to simplify the setup process!
  - For Windows and macOS users, Docker Desktop is the recommended way to install Docker. It provides a user-friendly interface and includes all the necessary components to run Docker on your machine.
    - Windows users will need [WSL 2](./wsl.md) to run Docker Desktop, which is a compatibility layer for running Linux binary executables natively on Windows 10 and later. It is automatically installed by Docker Desktop, so you don't need to follow the guide.
  - For Linux users, you can install Docker Engine directly from the command line.

### Prerequisites

- Access to a computer (preferably a laptop) with an internet connection

## Tutorial

### Windows

- Follow this link and follow the instructions there to install Docker Desktop on Windows: [https://docs.docker.com/desktop/install/windows-install/](https://docs.docker.com/desktop/install/windows-install/)
  - Make sure you install using the WSL 2 backend (this is the default option). If you have WSL 2 installed, Docker Desktop will automatically use it. If you don't have WSL 2 installed, Docker Desktop will prompt you to install it during the installation process.
  - You don't need to follow "Advanced system configuration and installation options" or anything after it. Once Docker Desktop is running, you're good to go.

### macOS

- Follow this link and follow the instructions there to install Docker Desktop on macOS: [https://docs.docker.com/desktop/install/mac-install/](https://docs.docker.com/desktop/install/mac-install/)

### Linux

- Follow this link and follow the instructions there to install Docker Engine on Linux: [https://docs.docker.com/engine/install/](https://docs.docker.com/engine/install/)
