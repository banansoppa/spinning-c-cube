# spinning-c-cube
A very simple C program that renders 3 dimensional points and lines in the terminal. It renders a rectangular cuboid (2x2x3).
Build with `gcc -lm -o 3d 3d.c`
The arguments to the program is `./3d FRAMES ANGLE1 ANGLE2 WIDTH HEIGHT SCALE_X SCALE_Y`, you have to provide either 0, 1, 3 or 7 arguments.
FRAMES are amount of frames to render, the default of this value is 200.
ANGLE1 and ANGLE2 are the points starting rotation around the Y axis and Z axis respectively, the parameter is an int and the actual angle is `ANGLE*PI/10`. By default these are 3pi/10 and 4pi/10 respectively.
WIDTH, HEIGHT are the widths and heights of the output, in letters, the default is 40, 20. SCALE_X and SCALE_Y is how much the output will be stretced, in the X and Y cardinal directions. This is necessary since most fonts are slightly taller than wide, the defaults values are 14, 10.
