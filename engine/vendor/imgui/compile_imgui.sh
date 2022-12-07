#!/bin/bash
g++ -m64 -O3 -std=c++2a -c imgui.o *.cpp -lGL -lglfw -lpthread -lm -lvulkan