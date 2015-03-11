#packages
sudo apt-get install freeglut3-dev

# to compile
make

# to start
./tdp_game images/test.pgm images/test.ppm images/arrow_right.ppm

KEYS:
arrows   : move the agent (issuing translational and rotational forces)
+/-      : zoom/unzoom
pgup/dwn : move the camera along the z axis

1/2/3    : toggle view (local out, local in, global)
