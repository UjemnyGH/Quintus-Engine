echo "To add more files edit run.sh file and add yours files to g++"
echo "Compiling Editor"
g++ -m64 -O3 -std=c++2a -o Editor ../../engine/vendor/glad/glad.c ../../engine/vendor/imgui/*.cpp src/*.cpp -lGL -lglfw -lpthread -lm -lvulkan
echo "Running Editor"
./Editor

