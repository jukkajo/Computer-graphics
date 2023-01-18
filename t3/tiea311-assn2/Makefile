#
# Adapted from the original MIT assigment:
#
.PHONY: all clean veryclean
INCFLAGS  = -I /usr/include/GL
INCFLAGS += -I ./vecmath/include
INCFLAGS += -I ./fltk-1.3.8/src
INCFLAGS += -I ./fltk-1.3.8/FL
#INCFLAGS += -I /usr/bin/X11

LINKFLAGS  = -lglut -lGL -lGLU 
LINKFLAGS += -L ./ -lvecmath
LINKFLAGS += -lfltk -lfltk_gl -lX11 -lXft -lXext -lsupc++ -lfontconfig -lXrender

CFLAGS    = -g -Wall -DSOLN
CC        = g++
SRCS      = bitmap.cpp camera.cpp MatrixStack.cpp modelerapp.cpp modelerui.cpp ModelerView.cpp Joint.cpp SkeletalModel.cpp Mesh.cpp main.cpp
SRCS      += $(wildcard vecmath/src/*.cpp)

OBJS      = $(SRCS:.cpp=.o)
PROG      = a2

VECMATHSRC= Matrix2f.cpp  Matrix3f.cpp  Matrix4f.cpp  Quat4f.cpp  Vector2f.cpp  Vector3f.cpp  Vector4f.cpp
VECMATHOBJ=$(patsubst %.cpp,%.o,$(VECMATHSRC))

all: $(SRCS) $(PROG)

libvecmath.a: $(addprefix vecmath/src/,$(VECMATHSRC))
	g++ -c $(CFLAGS) $(INCFLAGS) $(addprefix vecmath/src/,$(VECMATHSRC))
	ar crf $@ $(VECMATHOBJ)

$(PROG): $(OBJS) libvecmath.a
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LINKFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -c -o $@ $(INCFLAGS)

depend:
	makedepend $(INCFLAGS) -Y $(SRCS)

clean:
	-rm $(OBJS) $(PROG) $(VECMATHOBJ) *~

veryclean: clean
	-rm libvecmath.a

bitmap.o: bitmap.h
camera.o: camera.h
Mesh.o: Mesh.h
MatrixStack.o: MatrixStack.h
modelerapp.o: modelerapp.h ModelerView.h modelerui.h bitmap.h camera.h
modelerui.o: modelerui.h ModelerView.h bitmap.h camera.h modelerapp.h
ModelerView.o: ModelerView.h camera.h
SkeletalModel.o: MatrixStack.h ModelerView.h Joint.h modelerapp.h

