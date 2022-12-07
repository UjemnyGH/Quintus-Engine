#!/bin/sh

echo "------Project generator------"
echo "Set project name:"
read PRNAME
echo "Setted project name:"
echo $PRNAME

mkdir $PRNAME
cd $PRNAME

echo "echo \"To add more files edit run.sh file and add yours files to g++\"
echo \"Compiling ${PRNAME}\"
g++ -m64 -O3 -std=c++2a -o ${PRNAME} ../../engine/vendor/glad/glad.c ../../engine/vendor/imgui/*.cpp src/*.cpp -lGL -lglfw -lpthread -lm -lvulkan -llua
echo \"Running ${PRNAME}\"
./${PRNAME}
" > run.sh
chmod +x run.sh

mkdir src
cd src
echo "#include \"../../../engine/quintus.hpp\"

class Game : public qe::Window {
public:
    virtual void Start() override;
    virtual void Update() override;
};

void Game::Start() {
    // Code here
}

void Game::Update() {
    // Code here
}

void Application() {
    Game game;

    game.run();
}" > main.cpp