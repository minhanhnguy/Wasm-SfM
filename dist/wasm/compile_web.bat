@echo off
echo Compiling WebAssembly...
emcc -O3 sfm.cpp -s WASM=1 -s EXPORTED_FUNCTIONS="['_orb', '_convert_to_gray', '_malloc_image', '_free_image']" -s EXPORTED_RUNTIME_METHODS="['ccall', 'cwrap']" -o sfm.js
echo Compilation complete! 