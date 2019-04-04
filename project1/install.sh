#bin/bash

echo "Clonning repository"
mkdir lib
cd lib
git clone https://github.com/json-c/json-c.git

echo "Building static library"
cd json-c
sh autogen.sh
mkdir build
cd build
cmake ../ -DBUILD_SHARED_LIBS=OFF
make
cp libjson-c.a ../../../

echo "Compiling library"
cd ..
sh autogen.sh
./configure
make

cd ../..
rm ltmain.sh
