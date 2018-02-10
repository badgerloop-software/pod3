.PHONY: clean pv nav pi dash all
.DEFAULT_GOAL := all

# Toolchain configurations
include toolchain.mk

# Objects and target dependencies
include proc/conf.mk
include app/conf.mk
include drivers/conf.mk
include common/conf.mk

ALL_TARGETS += pi
pi: $(PI_DEPS)
	+@echo "" && echo "--- Raspberry Pi assets built ---" && echo ""

ALL_TARGETS += pv
pv: $(OBJ_DIR)/pv.bin
	+@echo "" && echo "--- Pressure Vessel module firmware built ---"
	+@echo ""

ALL_TARGETS += nav
nav: $(OBJ_DIR)/nav.bin
	+@echo "" && echo "--- Navigation module firmware built ---"
	+@echo ""

ALL_TARGETS += dash
dash: $(OBJ_DIR)/dash.bin
	+@echo "" && echo "--- Dashboard module firmware built ---"
	+@echo ""

all: $(ALL_TARGETS)
	+@echo "" && echo "=== All targets ($(ALL_TARGETS)) built ==="

include rules.mk
