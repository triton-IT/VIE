mkdir build

cd build

cmake -S ../ -B ./ -DCYCFI_ELEMENTS_DIR="D:/Projects/TritonIT/dependencies/cycfi/elements" -DSTEINBERG_VST3SDK_DIR="D:\Projects\TritonIT\dependencies\steinberg\vst3sdk-3.7.2\vst3sdk" -DCYCFI_Q_DIR="D:\Projects\TritonIT\dependencies\cycfi\q" -DNLOHMANN_JSON_DIR="D:\Projects\TritonIT\dependencies\nlohmann" -DCMAKE_BUILD_TYPE=Debug

cmake --build .