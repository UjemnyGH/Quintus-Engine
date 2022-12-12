echo "To add more files edit run.sh file and add yours files to g++"
echo "Compiling Example"
g++ -m64 -O3 -std=c++2a -o Example src/main.cpp ../../engine/vendor/glad/glad.c -lGL -lglfw -lpthread -lm -lvulkan -llua
echo "Running Example"
./Example

