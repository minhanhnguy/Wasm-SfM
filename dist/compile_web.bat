@echo off
echo Compiling WebAssembly...
emcc -O3 image_input.cpp -s WASM=1 -s EXPORTED_FUNCTIONS='["_convert_to_gray", "_malloc_image", "_free_image"]' -s EXPORTED_RUNTIME_METHODS="['ccall', 'cwrap']" -o image_input.js
echo Compilation complete! 