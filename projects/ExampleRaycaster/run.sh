echo "To add more files edit run.sh file and add yours files to g++"
echo "Compiling ExampleRaycaster"
g++ -O3 -std=c++2a -o ExampleRaycaster src/main.cpp -lGL -lGLEW -lglfw -lpthread -lm
echo "Running ExampleRaycaster"
./ExampleRaycaster

