CPU_DEFINES += -D__START=main

SRCS += common/startup.c
SRCS += common/board.c
SRCS += common/pcbuffer.c
SRCS += common/retarget.c
SRCS += common/console.c
SRCS += common/post.c
SRCS += common/pin_alias.c
SRCS += common/dashboard_control.c
SRCS += common/state_handlers.c
SRCS += common/state_machine.c
SRCS += common/solenoid.c
SRCS += common/dashboard_data.c
SRCS += common/nav_data.c
SRCS += common/pv_data.c
SRCS += common/data_set.c

include common/commands/conf.mk
