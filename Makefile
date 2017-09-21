
OS := $(shell uname -s)

ifeq ($(OS), Linux)
  FLAGS = -I/usr/include -L/usr/lib -L/usr/lib/nvidia-375 -lglut -lGLEW -lGL -lGLU -lX11 -Wno-write-strings
endif

ifeq ($(OS), Darwin)
  FLAGS = -framework Cocoa -framework OpenGL -framework GLUT -I/usr/local/opt/glew/include -I/opt/X11/include -L/usr/local/opt/glew/lib -L/opt/X11/lib -lGLEW -lGLUT -Wno-write-strings 
endif

%: %.cpp
	g++ -std=c++11 $*.cpp -o $* $(FLAGS)
