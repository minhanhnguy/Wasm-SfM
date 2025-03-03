@echo off
echo Compiling C++ Code...
g++ main.cpp ./convert_to_gray_scale/convert_to_gray_scale.cpp ./marking/marking.cpp ./orb/orb.cpp ./stb_image_implementation.cpp -o main.exe
echo Compilation complete!
