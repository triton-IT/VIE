mkdir build
cd build
cmake -S "D:\Projects\TritonIT\Vie" -B "D:\Projects\TritonIT\Vie\build" -G "Visual Studio 16 2019" -A "x64" -DCYCFI_ELEMENTS_DIR="D:/Projects/TritonIT/dependencies/cycfi/elements" -DSTEINBERG_VST3SDK_DIR="D:\Projects\TritonIT\dependencies\steinberg\vst3sdk-3.7.2" -DCYCFI_Q_DIR="D:\Projects\TritonIT\dependencies\cycfi\q" -DNLOHMANN_JSON_DIR="D:\Projects\TritonIT\dependencies\nlohmann"
cmake --build .