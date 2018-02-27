board-present:
	@lsusb | grep ST-LINK >/dev/null
	@lsblk -f | grep NODE_L432KC >/dev/null

install: board-present all
	@./build.py

flash: board-present all
	@./build.py flash

mount: board-present all
	@./build.py mount

# lsof /dev/port to see if something is using it
serial: board-present all
	@screen /dev/ttyACM0 115200,cs8,-parenb,-cstopb,-hupcl
