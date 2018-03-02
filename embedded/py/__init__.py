""" Package Identifier """
TOOLCHAIN = "arm-none-eabi-"
EDITOR = "vim"

# Only support four concurrent devices for now
MNT_POINTS = [
    "/tmp/nucleo1", "/tmp/nucleo2",
    "/tmp/nucleo3", "/tmp/nucleo4"
]
