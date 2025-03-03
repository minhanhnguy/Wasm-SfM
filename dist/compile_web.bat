@echo off
echo Compiling WebAssembly...
emcc -O3 math_lib.cpp -s WASM=1 -s EXPORTED_FUNCTIONS="['_add', '_subtract', '_multiply', '_divide']" -s EXPORTED_RUNTIME_METHODS="['ccall', 'cwrap']" -o math_lib.js
echo Compilation complete! 