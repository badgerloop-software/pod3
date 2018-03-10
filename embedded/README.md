# Badgerloop Pod: Embedded System

Firmware assets for Badgerloop-pod microcontroller modules.

This project currently supports one target ([NUCLEO-L432KC development board](http://www.st.com/en/evaluation-tools/nucleo-l432kc.html)) with no plans to support additional targets.
Multiple binary outputs (i.e. separate programs) are produced and are intended to be flashed to different individual electrical system modules.

[GNU make](https://www.gnu.org/software/make/manual/make.html) is used to
handle compilation via the
[GNU Arm Embedded Toolchain](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads).
Intermittent cleaning is not necessary by leveraging the compiler toolchain's ability to produce empty make recipes for each source file that declare the dependencies for each file (`-MM -MT` flags, \*.d files).

A Python program ([build.py](build.py)) is available to provide useful abstractions for development tasks such as flashing devices and interacting with serial ports.

## Workstation Requirements

This repository supports a remote-server development workflow with multiple simultaneous users. For Badgerloop, instructions to connect to the development server can be provided by Vaughn Kottler, Ryan Castle or Ethan Link.

Local workstation development is currently only supported for Linux-based systems and is only tested on [Ubuntu 16.04 LTS](http://releases.ubuntu.com/16.04/) (server and GNOME desktop).

The following software packages are required for local development:

* `gcc-arm-none-eabi` (Version 4.9.x)
* `gdb-arm-none-eabi` (Version 7.10)
* `make` (Version 4.1)
* `python` (Version 2.7.x)

Some additional configuration may be required to allow non-root access to serial streams and USB devices. For server installations, `/etc/fstab` entries are used to allow the STM32 Nucleo's to be mounted to a known location automatically (required for [build.py](build.py)).

## Directory Structure

TODO

## build.py Commands

TODO

## Make Targets

**(default, no argument)**

  * TODO
