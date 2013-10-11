#! /bin/bash

set -xe

CXX="${CXX-g++}"
MYCXXFLAGS="-std=c++11 `pkg-config --cflags sdl2 SDL2_image`"
MYLDFLAGS="-std=c++11 `pkg-config --libs sdl2 SDL2_image`"

mkdir -p build
rm -f build/*.o
for i in *.cpp; do
  "$CXX" $CXXFLAGS $MYCXXFLAGS -c -o "build/$i.o" "$i" &
done

wait
"$CXX" $LDFLAGS $MYLDFLAGS -o a.out build/*.o
