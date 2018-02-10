TOOLCHAIN = arm-none-eabi-
INCLUDES = -I proc -I include -I include/cmsis
CFLAGS = $(FPU) $(ARCH_FLAGS) $(DEFINES) $(CPU_DEFINES) $(INCLUDES)
CFLAGS += -Wall -Wextra -ffunction-sections -fdata-sections
CFLAGS += -fno-builtin -Os
CXXFLAGS=$(CFLAGS) -std=gnu++14

LFLAGS = --specs=nosys.specs -Wl,--gc-sections -Wl,-Map=$(PROJECT).map
LFLAGS += -Tproc/link.ld

OBJ_DIR = bin
