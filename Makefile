# This is a make file for OnlineForest package
# Amir Saffari, amir@ymer.org
#-----------------------------------------

# Compiler options
CC = g++

INCLUDEPATH = -I/usr/local/include
LINKPATH = -L/usr/local/lib

# PROFILING
#CFLAGS = -c -pg -O0 -Wall
#LDFLAGS = -lconfig++ -pg -latlas -llapack

# DEBUG
#CFLAGS = -c -g -O0 -Wall
#LDFLAGS = -lconfig++ -latlas -llapack

# OPTIMIZED
CFLAGS = -c -O3 -Wall -march=native -mtune=native -DNDEBUG
LDFLAGS = -lconfig++ -latlas -llapack

# Source directory and files
SOURCEDIR = src
HEADERS := $(wildcard $(SOURCEDIR)/*.h)
SOURCES := $(wildcard $(SOURCEDIR)/*.cpp)
OBJECTS := $(SOURCES:.cpp=.o)

# Target output
BUILDTARGET = Online-Forest

# Build
all: $(BUILDTARGET)
$(BUILDTARGET): $(OBJECTS) $(SOURCES) $(HEADERS)
	$(CC) $(LINKPATH) $(LDFLAGS) $(OBJECTS) -o $@
.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDEPATH) $< -o $@

clean:
	rm -f $(SOURCEDIR)/*~ $(SOURCEDIR)/*.o
	rm -f $(BUILDTARGET)
