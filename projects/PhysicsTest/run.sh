echo "To add more files edit run.sh file and add yours files to g++"
echo "Compiling PhysicsTest"
g++ -m64 -O3 -std=c++2a -c src/*.cpp -lGL -lglfw -lpthread -lm -lvulkan
g++ -m64 -O3 -std=c++2a -o PhysicsTest ../../engine/vendor/glad/*.o ../../engine/vendor/imgui/*.o ../../engine/vendor/lua/*.o *.o -lGL -lglfw -lpthread -lm -lvulkan
rm ./*.o
echo "Running PhysicsTest"
./PhysicsTest

