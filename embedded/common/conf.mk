CPU_DEFINES += -D__START=main

SRCS += common/startup.c
SRCS += common/board.c
SRCS += common/pcbuffer.c
SRCS += common/retarget.c
SRCS += common/console.c
SRCS += common/post.c
SRCS += common/pin_alias.c
SRCS += common/dashboard_control.c
SRCS += common/bms.c
SRCS += common/rms.c
SRCS += common/badgerloop.c
include common/commands/conf.mk
