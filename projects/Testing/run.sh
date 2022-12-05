echo "To add more files edit run.sh file and add yours files to g++"
echo "Compiling Testing"
g++ -m64 -O3 -std=c++2a -o Testing ../../engine/vendor/glad/glad.c src/main.cpp -lGL -lglfw -lpthread -lm
echo "Running Testing"
./Testing

