#!/bin/sh

.zig/zig cc -o test-app include/glad/glad.c src/main.c -I./include/ $(pkg-config --libs gl) $(pkg-config --cflags --static --libs glfw3)
