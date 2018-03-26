.PHONY: clean pv nav pi dash dev all install board-present unmount flash mount serial dump debug
.DEFAULT_GOAL := install

REVISION := 0

# Toolchain configurations
include mk/toolchain.mk

# Objects and target dependencies
include proc/conf.mk
include app/conf.mk
include drivers/conf.mk
include common/conf.mk

OBJECTS = $(SRCS:.c=.o)

# Dependency generation
DEPS += $(SRCS:.c=.d)
DEPS += $(DASH_DEPS:.o=.d)
DEPS += $(PV_DEPS:.o=.d)
DEPS += $(NAV_DEPS:.o=.d)
DEPS += $(DEV_DEPS:.o=.d)
-include $(DEPS)

include mk/rules.mk
include mk/install.mk
