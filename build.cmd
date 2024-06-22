rd /s /q build 2> NUL
mkdir build
cd build
cmake ..
cd ..
cmake --build build --config Release --target ALL_BUILD
