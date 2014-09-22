Usage:

Project and solution files are VS 2013. Static linking used for debug and release executables, so it should work without any dependency.
EXE files are in debug and release folders.(along with dependency glut32.dll)

Initial setup:
On the console, please enter parameters as asked.
Obj file should be in current directory."cube2.obj" is there in all working directories for now.
Enter 0 to stick to using cube2.obj.

Note: windows 8 might block the exe file from executing. Click more info -> run anyway option.

Render:
Ray caster works only with ray triangle intersections.

Limitations:
Make sure window width and height are greater than 256. I believe this is a glut bug.

Credits to external sources:

1. Code used:

a. Tiny Obj Loader: (with minor modifications so materials are loaded correctly)
For parsing obj files.
https://github.com/syoyo/tinyobjloader

b. GLUT:
For creating a window and rendering the pixels from my colorbuffer.

c. GLM:
For all vector calculations.

2. Other sources:

a. Ray triangle instersection: 
http://www.scratchapixel.com/lessons/3d-basic-lessons/lesson-9-ray-triangle-intersection/ray-triangle-intersection-geometric-solution/
