# Documentation Repository Setup
## Overview
- This document contains instructions on how to set up the PR-Docs repository (this one) for development and contribution. 

### Operating Systems
- If you are on Windows, using WSL is strongly recommended for development with Python and Sphinx.
	- Follow the [WSL tutorial here](./wsl).
- If you are on MacOS, it is assumed that you know how to work with the terminal and can figure out any differences from the standard Linux commands and procedures.
- If you are on Linux, good. The rest of this tutorial assumes you are using Linux or WSL. However, some notes may be redundant for you since this tutorial assumes little to no Linux experience.

## Preparing
- First, clone [this repo](https://github.com/PolarRobotics/PR-Docs) (`PR-Docs`) to a suitable location on your computer. 
	- See the [Git and GitHub training](./git) for a refresher on how to clone a repository.
	- **Make note of the location you clone to.** 
	- For Windows, this tutorial assumes you use the default location of `C:\Users\<YourUsername>\Documents\GitHub`, but realistically you should use a different directory.
		- For the purposes of this tutorial, "directory" is essentially synonymous with "folder" that you may be used to.
		- `<YourUsername>` should be replaced with whatever your Windows username is.
- If you have not done so already, open a new terminal (or WSL instance).
	- Note that you can open a terminal (including WSL) from within VSCode.
- On startup, your current working directory should be `~`, which is your 'home' directory.
	- You can view your current working directory at any time by using the command `pwd` (print working directory).
	- If on WSL, this is inside your Linux environment, which is effectively separate from the rest of your computer.
- A few things to note:
	1. You can view files and folders in the current directory using `ls`.
	2. `.` refers to the current directory (visible via `pwd`)
	3. `..` refers to the directory one level above (the directory containing the current directory).
- Using `cd`, change your current directory to the location you cloned this repo to.
	- In WSL, you can do this with `cd /mnt/c/Users/<YourUsername>/Documents/GitHub`. Note that Linux filesystems use forward slashes, while Windows uses backslashes (but will generally convert forward slashes to backslashes).
	- On Linux or MacOS, you should be able to navigate to the target location from your home directory more easily.
- Change your working directory to `./PR-Docs/docs` (or `/docs` with respect to the repository).

## Preparing VSCode
- You should install the following VSCode Extensions into your VSCode Workspace (or globally, but it is recommended to use workspaces to avoid having all extensions active all the time).
	- [MyST-Markdown](https://marketplace.visualstudio.com/items?itemName=ExecutableBookProject.myst-highlight)
	- [myst-lsp](https://marketplace.visualstudio.com/items?itemName=chrisjsewell.myst-lsp)
	- [Markdown All in One](https://marketplace.visualstudio.com/items?itemName=yzhang.markdown-all-in-one)
	- [Markdown Table](https://marketplace.visualstudio.com/items?itemName=TakumiI.markdowntable)
	- [Markdown Table Prettify](https://marketplace.visualstudio.com/items?itemName=darkriszty.markdown-table-prettify)

## Python
### Installing Python
- Update your packages via your package manager.
	- For Ubuntu, run `sudo apt-get update` and then `sudo apt-get upgrade`.
- Install `python3` and `pip` via your package manager.
	- For Ubuntu, run `sudo apt-get install python3` followed by `sudo apt-get install python3-pip`.
	- For MacOS, you will need to install Homebrew, and use `brew` to install these packages.

### Creating a Virtual Environment
- It is **strongly recommended** to create a virtual environment to isolate this repo's dependencies from the rest of your system.
- First, `cd` to the root of the repository (`PR-Docs`). 
	- If you're in `docs`, just run `cd ..` to navigate up one level.
- Run the following commands individually (i.e., not all at once).
	- This assumes Ubuntu, if using another distro replace the first line with your package manager.
```sh
sudo apt-get install python3-venv
python3 -m venv ./.venv
```

- At this point you will have created a Python *virtual environment* named `.venv`. 
- To *activate* the virtual environment, run the following command.
	- After doing so, there should be a `(.venv)` prefix in your terminal prompt.
	- Remember this command, as you will have to use it frequently (each time you want to build the documentation).
```sh
source ./.venv/bin/activate
```
- If you have successfully activated hte virtual environment, your prompt should have the `(.venv)` prefix, like this:
```
(.venv) user@machine: /mnt/c/.../PR-Docs/docs$ 
```
- The rest of this tutorial assumes that you created and activated a virtual environment named `.venv`.

### Installing Packages
- First, `cd docs`.
- If you have not done so already, 
- You can install all requisite packages by running:
```sh
pip install -r requirements.txt
```

- Alternatively, run the commands below sequentially:
```sh
pip install sphinx
pip install sphinx-book-theme
pip install myst-parser
pip install "rst-to-myst[sphinx]"
pip install sphinx-notfound-page
pip install sphinx-copybutton
pip install sphinx-togglebutton
```

- You may also be able to install `sphinx` or `python3-sphinx` directly from your package manager.
- To verify that sphinx installed correctly, run:
```sh
sphinx-build --version
```

- For completeness, the command to start a new documentation project/repo is `sphinx-quickstart`, but **do not run this command** in this repo.

## Development
- Congratulations! You should now be able to begin developing in this repository.
- In order to build (compile) the documentation and run it locally to test your changes, you can run `./build-run` from the `docs` folder to perform both actions automatically.
	- This will invoke the sphinx build process, and once that is complete, it will launch a simple Python web server.
	- You will then be able to visit `localhost:8000` in your browser to see the documentation.

### Manually Building the Docs
- In general your working directory should be `/docs`. You can check this with `pwd`.
- From `/docs`, you will be able to build the site by running `make clean html`.
	- You can also run `make.bat` from Windows Powershell from the same directory using `./make clean html`.
- You can edit the files in the directory normally with Obsidian or VSCode natively from your host operating system, but you will need to build them using the terminal/WSL.

### Manually Launching the Webserver
- The webserver should be launched from the directory `/docs/build/html`.
- To launch the webserver while working in the `docs` directory, run the following command:
```sh
python3 -m http.server -d build/html
```
- `-m` specifies a *module name*, in this case, the HTTP server module, which launches a local web server.
- `-d`, or `--directory`, specifies a relative location to launch the webserver from.
- You can run this command more easily via `./serv` from `/docs`.

## Resources
- [https://www.sphinx-doc.org/en/master/usage/markdown.html](https://www.sphinx-doc.org/en/master/usage/markdown.html)
- [https://cerodell.github.io/sphinx-quickstart-guide/build/html/markdown.html](https://cerodell.github.io/sphinx-quickstart-guide/build/html/markdown.html)
- [https://myst-parser.readthedocs.io/en/latest/index.html](https://myst-parser.readthedocs.io/en/latest/index.html)
- [https://docs.readthedocs.io/en/latest/tutorial/index.html](https://docs.readthedocs.io/en/latest/tutorial/index.html)
- [https://www.writethedocs.org/guide/](https://www.writethedocs.org/guide/)