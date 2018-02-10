CPU_DEFINES += -DSTM32L4 -DSTM32L432xx -DARMCM4_DP -DARMCM4
FPU = -mfpu=fpv4-sp-d16 -mfloat-abi=softfp
ARCH_FLAGS = -mthumb -march=armv7e-m

SRCS += proc/system.c
SRCS += proc/vectors.c
