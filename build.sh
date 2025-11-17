#!/bin/bash

set -euo pipefail

# sh-unknown-elf-gcc -m2e -nostdlib -nostartfiles -Ttext=0x0 -Wl,-e,_start -o ecu.elf ecu.S
sh-unknown-elf-as -o ecu.o ecu.S
sh-unknown-elf-ld -T link.ld -e _start -o ecu.elf ecu.o
sh-unknown-elf-objdump -D -S ecu.elf > ecu.dis
sh-unknown-elf-objcopy -O binary ecu.elf ecu.bin