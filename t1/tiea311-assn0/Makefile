# GNU makefile for those who use Linux. May need some changes to work on
# your own personal machine.
#
# Adapted from the original MIT assigment:
# - Naturally, we do not have "/mit/6.837/public/" so we need local build
# - Added the full source code of the vecmath library as given in 2012.
# - Build libvecmath.a to current working directory and link from there.
# - Updated also clean target; added "veryclean" to rid of library.
#
.PHONY: all clean veryclean
INCFLAGS  = -I /usr/include/GL
INCFLAGS += -I ./vecmath/include

LINKFLAGS = -lglut -lGL -lGLU
LINKFLAGS += -L ./ -lvecmath

CFLAGS    = -O2 -Wall -ansi -DSOLN
CC        = g++
SRCS      = main.cpp
OBJS      = $(SRCS:.cpp=.o)
PROG      = a0

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
