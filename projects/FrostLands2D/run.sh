echo "To add more files edit run.sh file and add yours files to g++"
echo "Compiling FrostLands2D"
g++ -m64 -O3 -std=c++2a -o FrostLands2D ../../engine/vendor/glad/*.o ../../engine/vendor/imgui/*.o src/*.cpp -lGL -lglfw -lpthread -lm -lvulkan -llua
echo "Running FrostLands2D"
./FrostLands2D

