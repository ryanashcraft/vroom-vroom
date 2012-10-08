
CC=g++
incs=vroomvroom.cpp
target=vroomvroom
v8=~/Apps/Libraries/V8/out/ia32.release/obj.target/tools/gyp/libv8_base.a
v8+=~/Apps/Libraries/V8/out/ia32.release/obj.target/tools/gyp/libv8_snapshot.a
v8incs=/home/ryanashcraft/Apps/Libraries/V8/include
cflags=-std=c++11 -g
lflags=-lpthread

all:
	$(CC) -I$(v8incs) $(cflags) $(incs) -o $(target) $(v8) $(lflags)

