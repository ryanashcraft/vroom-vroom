
CC=g++
sources=source/*.cpp
inc=include/
incs=include/* source/*
bins=bin/*
target=vroomvroom
v8=~/Apps/Libraries/V8/out/ia32.release/obj.target/tools/gyp/libv8_base.a
v8+=~/Apps/Libraries/V8/out/ia32.release/obj.target/tools/gyp/libv8_snapshot.a
v8incs=/home/ryanashcraft/Apps/Libraries/V8/include
cflags=-std=c++11 -g -I$(inc)
lflags=-lpthread

all:
	$(CC) -I$(v8incs) $(cflags) $(sources) -o $(target) $(v8) $(lflags)

clean:
	rm $(target) $(bins) 

