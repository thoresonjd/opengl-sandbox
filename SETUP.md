# Setup

This file outlines the steps necessary for the creation of this project repository.

## Initializing Project Repository

- Create an empty repository on GitHub
- Create an empty C++ project with Visual Studio
	- Ensure that "place solution and project in the same directory" is checked
- Configure local Git repository within project directory
	```
	git init
	```
- Link local repository to remote GitHub repository
	```
	git remote add origin <GitHub repo URL>
	git remote -v
	```
- Create four directories named `/src`, `/include`, `/lib`, and `/dll`
	- `/src` will contain source code files for the project
	- `/include` will contain header files and header-only libraries
	- `/lib` will contain library files and implementation files
	- `/dll` will contain dynamic-link library files
- Configure propject properties in Visual Studio
	- Output files\
		*Project > Properties > Configuration Properties > General*
		```
		Output Directory: $(SolutionDir)bin\$(Platform)\$(Configuration)\
		Intermediate Directory: $(SolutionDir)bin\intermediates\$(Platform)\$(Configuration)\
		```
	- Locate include files\
		*Project > Properties > Configuration Properties > C/C++ > General*
		```
		Additional Include Directories: $(SolutionDir)include\
		```
	- Locate library files\
		*Project > Properties > Configuration Properties > Linker > General*
		```
		Additional Library Directories: $(SolutionDir)lib\
		```
	- Locate dynamic-link library files\
		*Project > Properties > Configuration Properties > Debugging*
		```
		Environment: PATH=$(SolutionDir)dll\
		```
	- Specify dependencies\
		*Project > Properties > Configuration Properties > Linker > Input*
		```
		Additional Dependencies: $(CoreLibraryDependencies);%(AdditionalDependencies)
		```
- Create basic project configuration files
	- *README.md*
	- *.gitignore*
		```
		# output binaries
		bin/

		# Visual Studio
		.vs/
		*.sln
		*.vcxproj*

		# other files as necessary
		...
		```
- Create a simple "Hello, world!" program to ensure project is properly configured

## Setting Up OpenGL

- Download and decompress the latest version of GLFW from https://www.glfw.org/
	- This project uses the 64-bit Window binaries
- In the downloaded folder:
	- Find the `/include` directory and copy the `/GLFW` folder to the project's `/include` directory
	- Find `glfw3.lib` for the desired Visual Studio version and add it to `/lib`
- Download GLAD, the GLAD1 version, from https://github.com/Dav1dde/glad
- Add `/glad` and `/KHR` to `/include`
- Add `glad.c` to `/lib`
- Add `glfw3.lib` and `opengl32.lib` to linker dependencies\
	*Project > Properties > Configuration Properties > Linker > Input*
	```
	Additional Dependencies: glfw3.lib;opengl32.lib;$(CoreLibraryDependencies);%(AdditionalDependencies)
	```
- Create an OpenGL program that renders a window to ensure OpenGL is working