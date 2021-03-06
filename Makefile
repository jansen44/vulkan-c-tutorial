CFLAGS = -O2
LDFLAGS = -lglfw -lvulkan
BIN_NAME = hello-triangle

build: make-target compile

run: build
	./target/$(BIN_NAME)

compile: main.c vulkan.c
	gcc $(CFLAGS) -o target/$(BIN_NAME) main.c vulkan.c $(LDFLAGS)

make-target:
	mkdir -p target/