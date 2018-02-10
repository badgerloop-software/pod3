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

%.bin: %.elf
	@$(TOOLCHAIN)objcopy -O binary $< $@

clean:
	find . -name '*.o' -delete
	rm -rf $(OBJ_DIR)

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
