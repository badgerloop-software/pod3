TOOLCHAIN = arm-none-eabi-
INCLUDES = -I proc -I include -I include/cmsis -I include/st
CFLAGS = $(FPU) $(ARCH_FLAGS) $(DEFINES) $(CPU_DEFINES) $(INCLUDES)
CFLAGS += -Wall -Wextra -ffunction-sections -fdata-sections -Werror
CFLAGS += -fno-builtin -Os
CXXFLAGS=$(CFLAGS) -std=gnu++14

LFLAGS = --specs=nosys.specs -Wl,--gc-sections
LFLAGS += -Tproc/link.ld

OBJ_DIR = bin

GIT_TIME=$(shell git log -n 1 --date=iso --pretty=format:"%cd")
GIT_VERSION=$(shell git log -n 1 --pretty=format:"%h-%cn")
SHELL_TIME=$(shell date)
DEFINES+=-D_GIT_TIME="\"$(GIT_TIME)\"" -D_GIT_VERSION="\"$(GIT_VERSION)\""
DEFINES+=-D_SHELL_TIME="\"$(SHELL_TIME)\""
DEFINES+=-D__VERSION='$(REVISION)'
