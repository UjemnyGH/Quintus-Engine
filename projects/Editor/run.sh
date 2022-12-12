echo "To add more files edit run.sh file and add yours files to g++"
echo "Compiling Editor"
g++ -m64 -O3 -std=c++2a -c src/*.cpp -lGL -lglfw -lpthread -lm -lvulkan -llua
g++ -m64 -O3 -std=c++2a -o Editor ../../engine/vendor/glad/*.o ../../engine/vendor/imgui/*.o *.o -lGL -lglfw -lpthread -lm -lvulkan -llua
echo "Running Editor"
./Editor

