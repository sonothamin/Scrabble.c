cd build
chmod +x premake5
./premake5 gmake
cd ..
make clean
make
xdg-open ./bin/Debug/
