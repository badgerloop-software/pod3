board-present: all
	@lsusb | grep ST-LINK >/dev/null
	@lsblk -f | grep NODE_L432KC >/dev/null

install: board-present
	@./build.py

flash: board-present
	@./build.py flash

mount: board-present
	@./build.py mount

dump: all
	@./build.py dump

debug: board-present
	@./build.py debug

# lsof /dev/port to see if something is using it
SERIAL_BAUD = 115200
SERIAL_ARGS = $(SERIAL_ARGS),cs8,-parenb,-cstopb,-hupcl
serial: board-present
	@screen /dev/ttyACM0 $(SERIAL_ARGS)
