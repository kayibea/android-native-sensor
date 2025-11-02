NDK_ROOT := $(HOME)/AndroidX/Sdk/ndk/29.0.14206865
TOOLCHAIN := $(NDK_ROOT)/toolchains/llvm/prebuilt/linux-x86_64
SYSROOT := $(TOOLCHAIN)/sysroot
BIN := $(TOOLCHAIN)/bin

TARGET := aarch64-linux-android33
CC := $(BIN)/$(TARGET)-clang

CFLAGS := -Wall -Wextra -Werror -O2 --sysroot=$(SYSROOT)
LDFLAGS := -landroid -llog

all: list_sensors accel_demo

list_sensors: list_sensors.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

accel_demo: accel_demo.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f list_sensors accel_demo

.PHONY: clean
