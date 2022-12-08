echo "To add more files edit run.sh file and add yours files to g++"
echo "Compiling Testing"
#g++ -m64 -O3 -std=c++2a -o Testing ../../engine/vendor/glad/glad.c ../../engine/vendor/imgui/*.cpp src/*.cpp -lGL -lglfw -lpthread -lm -lvulkan
g++ -m64 -O3 -std=c++2a -c src/*.cpp -lGL -lglfw -lpthread -lm -lvulkan -llua
g++ -m64 -O3 -o Testing *.o ../../engine/vendor/imgui/*.o ../../engine/vendor/glad/*.o -lGL -lglfw -lpthread -lm -lvulkan -llua
rm ./*.o
echo "Running Testing"
./Testing

