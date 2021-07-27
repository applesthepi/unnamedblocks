cd vcpkg
call bootstrap-vcpkg.bat
vcpkg.exe upgrade
vcpkg.exe update
vcpkg.exe integrate install
vcpkg.exe install cpptoml:x64-windows
vcpkg.exe install freetype:x64-windows
vcpkg.exe install glad:x64-windows
vcpkg.exe install glfw3:x64-windows
vcpkg.exe install glm:x64-windows
vcpkg.exe install imgui:x64-windows
vcpkg.exe install rapidjson:x64-windows
vcpkg.exe install stb:x64-windows
vcpkg.exe install vulkan:x64-windows
vcpkg.exe install zstd:x64-windows
echo done