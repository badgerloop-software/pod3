board-present:
	@lsusb | grep ST-LINK >/dev/null
	@lsblk -f | grep NODE_L432KC >/dev/null

install: board-present all
	@./build.py

flash: board-present all
	@./build.py flash

mount: board-present all
	@./build.py mount
