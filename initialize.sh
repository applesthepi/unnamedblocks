#!/bin/bash

echo this may take a while
sleep 1s
git submodule update --init;
./vcpkg/bootstrap-vcpkg.sh;
./vcpkg/vcpkg upgrade;
./vcpkg/vcpkg update;
./vcpkg/vcpkg integrate install;
./vcpkg/vcpkg install cpptoml:x64-linux;
./vcpkg/vcpkg install freetype:x64-linux;
./vcpkg/vcpkg install glad:x64-linux;
./vcpkg/vcpkg install glfw3:x64-linux;
./vcpkg/vcpkg install glm:x64-linux;
./vcpkg/vcpkg install imgui:x64-linux;
./vcpkg/vcpkg install rapidjson:x64-linux;
./vcpkg/vcpkg install stb:x64-linux;
./vcpkg/vcpkg install vulkan:x64-linux;
./vcpkg/vcpkg install zstd:x64-linux;
echo done;