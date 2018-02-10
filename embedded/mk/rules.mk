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
%.d: %.c
	+@echo "generating '$(notdir $@)' $* $@"
	@set -e; rm -f $@; \
	$(TOOLCHAIN)gcc -MM -MT '$*.o' $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

%.bin: %.elf
	@$(TOOLCHAIN)objcopy -O binary $< $@

clean:
	@find . -name '*.o' -delete
	@find . -name '*.d' -delete
	@rm -rf $(OBJ_DIR)

# Target-Specific Recipes

$(OBJ_DIR)/pv.elf: $(OBJECTS) $(PV_DEPS)
	@$(TOOLCHAIN)gcc $(CFLAGS) $(LFLAGS) $^ -o $@
	+@echo "linking '$(notdir $@)'"

$(OBJ_DIR)/nav.elf: $(OBJECTS) $(NAV_DEPS)
	@$(TOOLCHAIN)gcc $(CFLAGS) $^ $(LFLAGS) -o $@
	+@echo "linking '$(notdir $@)'"

$(OBJ_DIR)/dash.elf: $(OBJECTS) $(DASH_DEPS)
	@$(TOOLCHAIN)gcc $(CFLAGS) $^ $(LFLAGS) -o $@
	+@echo "linking '$(notdir $@)'"

ALL_TARGETS += $(OBJ_DIR)/pv.bin
ALL_TARGETS += $(OBJ_DIR)/nav.bin
ALL_TARGETS += $(OBJ_DIR)/dash.bin

all: $(ALL_TARGETS)
