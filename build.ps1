cls

d:
cd 'D:\Projects\TritonIT\VIE'

if (Test-Path -Path 'build') {
    #Remove-Item -Recurse -Force build
}

if (Test-Path -Path 'bin') {
    #Remove-Item -Recurse -Force bin
}

mkdir build
cd build

cmake .. -DDEPLOY_TO_VST_FOLDER=TRUE -DDEBUG=TRUE

#cmake --build .

#../VST_SDK/build/bin/Debug/validator.exe "../../../../../VIE/build/src/VIE.vst3"