# Badgerloop Pod: Embedded System

Firmware assets for Badgerloop-pod microcontroller modules.

This project currently supports one target  
([NUCLEO-L432KC development board](http://www.st.com/en/evaluation-tools/nucleo-l432kc.html))  
with no plans to support additional targets. Multiple binary outputs  
(i.e. separate programs) are produced and are intended to be flashed to
different individual electrical system modules.

[GNU make](https://www.gnu.org/software/make/manual/make.html) is used to
handle compilation via the  
[GNU Arm Embedded Toolchain](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads).  
Intermittent cleaning is not necessary by leveraging the compiler toolchain's  
ability to produce empty make recipes for each source file that declare  
the dependencies for each file (`-MM -MT` flags, \*.d files).

A Python program ([build.py](build.py)) is available to provide useful  
abstractions for development tasks such as flashing devices and interacting  
with serial ports.

## Directory Structure

TODO

## build.py Commands

TODO

## Make Targets

**(default, no argument)**

  * TODO
