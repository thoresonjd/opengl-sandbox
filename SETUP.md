# Setup

This file outlines the steps necessary for the creation of this project
repository.

## Steps

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
- Configure Visual Studio
	- Output files\
		*Project > Properties > Configuration Properties > General*
		```
		Output Directory: $(SolutionDir)bin\$(Platform)\$(Configuration)\
		Intermediate Directory: $(SolutionDir)bin\intermediates\$(Platform)\$(Configuration)\
		```
	- Locate DLLs\
		*Project > Properties > Configuration Properties > Debugging*
		```
		Environment: PATH=$(SolutionDir)dll\
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
	- Specify Dependencies\
		*Project > Properties > Configuration Properties > Linker > Input*
		```
		Additional Dependencies: <dependencies>
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
- Stage and commit all desired files
	```
	git add <files>
	git commit -m <commit message>
	```
- Push commits to remote repository
	```
	git push origin main
	```