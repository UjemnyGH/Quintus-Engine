echo "To add more files edit run.sh file and add yours files to g++"
echo "Compiling Example2D"
g++ -m64 -O3 -std=c++2a -o Example2D src/main.cpp ../../engine/vendor/glad/glad.c -lGL -lglfw -lpthread -lm
echo "Running Example2D"
./Example2D

