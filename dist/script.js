let wasmReady = false;
let malloc_image, free_image, convert_to_gray, orb;

Module.onRuntimeInitialized = function() {
    console.log("WebAssembly Loaded...");
    malloc_image = Module.cwrap('malloc_image', 'number', ['number']);
    free_image = Module.cwrap('free_image', null, ['number']);
    convert_to_gray = Module.cwrap('convert_to_gray', null, ['number', 'number', 'number']);
    orb = Module.cwrap('orb', null, ['number', 'number', 'number']);
    wasmReady = true; 
};

function convertToPNG(file) {
    return new Promise((resolve, reject) => {
        const reader = new FileReader();
        reader.onload = function(e) {
            const img = new Image();
            img.onload = function() {
                const tempCanvas = document.createElement('canvas');
                tempCanvas.width = img.width;
                tempCanvas.height = img.height;
                const ctx = tempCanvas.getContext('2d');
                ctx.drawImage(img, 0, 0);
                tempCanvas.toBlob((blob) => {
                    resolve(blob);
                }, 'image/png');
            };
            img.onerror = reject;
            img.src = e.target.result;
        };
        reader.onerror = reject;
        reader.readAsDataURL(file);
    });
}

document.getElementById('upload').addEventListener('change', async function(event) {
    if (!wasmReady) {
        console.error("WASM module not yet initialized. Please wait...");
        return;
    }

    const file = event.target.files[0];
    if (!file) return;

    try {
        const processFile = file.type === 'image/jpeg' ? await convertToPNG(file) : file;
        
        const reader = new FileReader();
        reader.onload = function(e) {
            const img = new Image();
            img.onload = function() {
                processImage(img);
            };
            img.src = e.target.result;
        };
        reader.readAsDataURL(processFile);
    } catch (error) {
        console.error('Error processing image:', error);
    }
});

function processImage(img) {
    console.log("Processing Image...");

    const canvas = document.getElementById('canvas');
    const ctx = canvas.getContext('2d');

    canvas.width = img.width;
    canvas.height = img.height;
    ctx.drawImage(img, 0, 0);

    const imageData = ctx.getImageData(0, 0, img.width, img.height);
    const data = imageData.data;
    const dataSize = data.length;

    if (!malloc_image || !free_image || !convert_to_gray) {
        console.error("WASM functions not initialized yet.");
        return;
    }

    // Allocate memory in WebAssembly
    const ptr = malloc_image(dataSize);
    if (!ptr) {
        console.error('Memory allocation failed');
        return;
    }

    // Copy image data to WebAssembly
    Module.HEAPU8.set(data, ptr);

    // Process image
    orb(ptr, img.width, img.height);

    // Copy processed data back
    const processedData = new Uint8ClampedArray(Module.HEAPU8.subarray(ptr, ptr + dataSize));
    
    // Preserve alpha channel
    for (let i = 3; i < dataSize; i += 4) {
        processedData[i] = data[i];
    }

    imageData.data.set(processedData);

    // Free memory
    free_image(ptr);

    // Display the output
    ctx.putImageData(imageData, 0, 0);
}