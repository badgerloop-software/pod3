# Generic Recipes

$(OBJECTS): | $(OBJ_DIR)

$(OBJ_DIR):
	@[ ! -d $(OBJ_DIR) ] && mkdir $(OBJ_DIR) 

%.o: %.cpp
	+@echo "building '$(notdir $<)'"
	@$(TOOLCHAIN)g++ $(CXXFLAGS) -c -o $@ $<

%.o: %.c
	+@echo "building '$(notdir $<)'"
	@$(TOOLCHAIN)gcc $(CFLAGS) -c -o $@ $<

# From https://www.gnu.org/software/make/manual/html_node/Automatic-Prerequisites.html
# 
# The following line was removed:
# +@echo "generating '$(notdir $@)' $* $@"
#
# It may be confusing to the uninitiated why this step is taking place,
# so obfuscate it for now
%.d: %.c
	@set -e; rm -f $@; \
	$(TOOLCHAIN)gcc -MM -MT '$*.o' $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

%.bin: %.elf
	@$(TOOLCHAIN)objcopy -O binary $< $@

clean:
	@find . -name '*.o' -delete
	@find . -name '*.d' -delete
	@find ./py -name '*.pyc' -delete
	@rm -rf $(OBJ_DIR)

# Target-Specific Recipes

$(OBJ_DIR)/pv.elf: $(OBJECTS) $(PV_DEPS)
	@$(TOOLCHAIN)gcc $(CFLAGS) $^ $(LFLAGS) -Wl,-Map=$(OBJ_DIR)/pv.map -o $@
	+@echo "linking '$(notdir $@)'"

$(OBJ_DIR)/nav.elf: $(OBJECTS) $(NAV_DEPS)
	@$(TOOLCHAIN)gcc $(CFLAGS) $^ $(LFLAGS) -Wl,-Map=$(OBJ_DIR)/nav.map -o $@
	+@echo "linking '$(notdir $@)'"

$(OBJ_DIR)/dash.elf: $(OBJECTS) $(DASH_DEPS)
	@$(TOOLCHAIN)gcc $(CFLAGS) $^ $(LFLAGS) -Wl,-Map=$(OBJ_DIR)/dash.map -o $@
	+@echo "linking '$(notdir $@)'"

$(OBJ_DIR)/dev.elf: $(OBJECTS) $(DEV_DEPS)
	@$(TOOLCHAIN)gcc $(CFLAGS) $^ $(LFLAGS) -Wl,-Map=$(OBJ_DIR)/dev.map -o $@
	+@echo "linking '$(notdir $@)'"

pv: $(OBJ_DIR)/pv.bin
nav: $(OBJ_DIR)/nav.bin
dash: $(OBJ_DIR)/dash.bin
dev: $(OBJ_DIR)/dev.bin

all: pv nav dash dev
