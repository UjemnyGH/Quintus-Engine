echo "To add more files edit Makefile file and add yours file to g++"
echo "Compiling Example"
g++ -o Example src/main.cpp -lGL -lGLEW -lglfw -lpthread -lm
echo "Running Example"
./Example

