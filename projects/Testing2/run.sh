echo "To add more files edit run.sh file and add yours files to g++"
echo "Compiling Testing2"
g++ -m64 -O3 -std=c++2a -c src/*.cpp -lGL -lglfw -lpthread -lm -lvulkan -llua
g++ -m64 -O3 -std=c++2a -o Testing2 ../../engine/vendor/glad/*.o ../../engine/vendor/imgui/*.o *.o -lGL -lglfw -lpthread -lm -lvulkan -llua
echo "Running Testing2"
./Testing2

