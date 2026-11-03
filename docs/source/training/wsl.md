# Windows Subsystem for Linux
## Overview
- This tutorial will walk you through installing Windows Subsystem for Linux, or WSL. 
	- WSL is a type of lightweight virtual machine integrated tightly with Windows that allows easy usage of linux commands and filesystem formatting while maintaining access to your entire Windows filesystem via a mounted device.

## Installation
- Start by either:
	1. Searching for "Windows Subsystem for Linux" in the Microsoft Store and clicking `Get`.
	2. Opening Windows Terminal, Command Prompt, or Powershell and running the command `wsl --install`.
- You will likely be prompted to grant elevation/administrator-level access. Do so.
- By default, WSL will install Ubuntu. It is **strongly recommended** to stick with the default as WSL lacks compatibility for non-Ubuntu distributions.
	- Furthermore, the rest of this tutorial (and others) will focus primarily on Ubuntu/Debian-based distributions where Linux is applicable.
- You will need to reboot your computer after WSL has finished installing.
- Once complete, WSL will ask you to choose a UNIX username and password.
	- Choose a brief appropriate username. Your first name or ONU email address name will suffice.
		- Do not use a username with spaces.
	- Choose a password. It need not be overly long, as you will need to type it frequently.
		- Hopefully you already have a strong password for your host machine...
	- **When inputting your password, no characters will appear - this is normal.** Simply continue typing your desired password.
		- You will be asked to confirm your password by typing it again - this will help if you type it incorrectly.
- Once the previous steps have finished, you will be presented with a shell prompt that looks like this:

```sh
user@desktop:~$
```

- If you followed this tutorial from the [documentation setup guide](./docs-setup), return there now.