echo Y|rmdir /s build 
echo Y|rmdir /s Debug
cmake -B ./build
cmake --build ./build