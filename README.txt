Building the OpenGL examples using Visual Studio
-----------------------------------------------

(Linux users see the bottom of this file.)

Unpack intro.zip and open the .sln file with Visual Studio 2012.  It should build
right out of the box, i.e., all required libraries are included in the workspace.
If you are doing this in a lab, it will ask you to confirm a location for the 
database that supports code browsing.  This doesn't particularly matter in that it 
will recreate the database as needed.

Running the examples depends on having freeglut.dll and glew32.dll into your path.  
Copies of these dlls can be found in the intro\bin directory. To run a project 
from VS, you can right-click on it in the "Solution Explorer" and select 
Debug->Start new instance. Running the examples from VS should work out of the box, 
since the project properties have been set to prepend "..\bin" to the system path.
(This is set in the project properties under "Debugging".)

However, if there are errors loading or compiling the 
shaders, the error messages will go to the console and you won't see them, since
the console window will close when the program exits. Instead, I usually run directly
from a command shell.  Note that the VS workspace ("solution") has a top-level
Debug directory as well as a Debug directory within each project.  The intermediate
build files go into the project-level Debug directory, but the executables go into
the top-level Debug directory.  The shader files are located in the project directory, 
so that should be your working directory.  For example, to run gl_example1 you would
cd to the 'gl_example1' directory in your command shell and then execute the command

    ..\Debug\gl_example1.exe

For this to work, you'll need to get the dll's into your system path.  To do so 
temporarily (this works in the labs) just execute the command    

      set PATH=..\bin;%PATH%  
      
If you are using your own computer, you can either add the solution's bin
directory to your path, or you can copy these the two dlls into Windows\System32 or
Windows\SysWOW64. (Note: if you change the PATH variable while VS is open, you'll
have to restart VS in order to pick up the changes.)


    
Creating new projects
---------------------
    
To experiment with additional examples, you will want to create your own projects.
The needed include files and lib files are found in the include/ and lib/ directories
at the top level, and should be referred to by relative paths in VS.

1) Right-click on the solution in the Solution Explorer and select Add -> New Project.
   Project type should be Win32 Console Application.  

2) Right click on the project and go to 
   Properties -> Configuration Properties -> C/C++ -> General -> Additional Include Directories:
   Type in "..\include" (Note: DON'T browse to the include directory, or else
   VS will put in an absolute path instead of a relative path)
   
   Note that the include for "Angel.h" will in turn include the GLUT, GLEW, and
   OpenGL headers that are needed.

3) Properties -> Configuration Properties -> Linker -> General -> Additional Library Directories:
   Type in "..\lib\windows" for a 32 bit build or "..\lib\windows\x64" for a 64 bit
   build (which I haven't actually tried).

4) Properties -> Configuration Properties -> Linker -> Input -> Additional Dependencies:
   Add glew32.lib

5) (This is unnecessary if you have updated your system path to include the glew and glut dlls.) 
   Properties -> Configuration Properties -> Debugging -> Environment:
   Type in PATH=..\bin;%PATH% 
  
6) Right click on the project and go to Add -> Existing Item
   Browse to ..\Common\InitShader.c

7) If you are using an existing source file as a starting point, be sure to **COPY**
   it physically into the project directory.  Then expand the project in Solution 
   Explorer, right click on the Source Files, and go 
   to Add -> Existing Item to add it to the build.

8) Be sure to COPY over whatever shader files you are using into the new project
   directory. Optionally, you can right click on the Resource Files and go to Add -> Existing Item
   to make the workspace aware of them.  This is convenient because then you can double-click
   to open them in the VS editor.

Note that each project has a section called "Header Files".  Although it is optional
to do so, in the future you may want to add the required header files as "existing items"
here.  That way if you change a header, VS will detect the change and force a rebuild.

Note about linking the rest of the libraries
--------------------------------------------
You might wonder why we don't need to list GLUT or OpenGL or any other libraries 
in the linker dependencies.  That's because the freeglut include files contain
a bunch of pragmas to do that for us:

#       pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#       pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#       pragma comment (lib, "gdi32.lib")    /* link Windows GDI lib        */
#       pragma comment (lib, "winmm.lib")    /* link Windows MultiMedia lib */
#       pragma comment (lib, "user32.lib")   /* link Windows user lib       */

Cleaning projects
-----------------

Visual Studio generates a truly monstrous amount of data, primarily for the database
for the context-sensitive type information and so on. If you need to transfer a workspace
between machines, you will want to reduce it in size a bit (this intro.zip file went
from 28M to 800K). First, in VS, do a "Clean" on the entire solution.  Then close VS and:

Delete the top-level ipch directory.
Delete the .sdf file.
Delete the .suo file.

Optionally (these don't actually take up much space):
Delete any .user, .ncb, .sbr, or .*log files


Building the OpenGL examples for Linux
--------------------------------------
You'll have to do some investigation to get the right libraries for GLUT and GLEW and replace
the ones in the lib directory.  You'll need to get the .so files into LD_LIBRARY_PATH too.
There is a makefile in each project directory that is adapted from the code samples provided by
the author.  These were tested a couple of years back using gnu make and g++ running 
under Cygwin.  Linux should be similar.

