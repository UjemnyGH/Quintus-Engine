echo "To add more files edit run.sh file and add yours files to g++"
echo "Compiling ExampleRaycaster"
g++ -m64 -O3 -std=c++2a -o ExampleRaycaster src/main.cpp ../../engine/vendor/glad/glad.c -lGL -lglfw -lpthread -lm
echo "Running ExampleRaycaster"
./ExampleRaycaster

