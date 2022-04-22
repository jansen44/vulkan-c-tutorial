CFLAGS = -O2
LDFLAGS = -lglfw -lvulkan
BIN_NAME = hello-triangle

build-and-run: make-target build
	./target/$(BIN_NAME)

build: main.c
	gcc $(CFLAGS) -o target/$(BIN_NAME) main.c $(LDFLAGS)

make-target:
	mkdir -p target/