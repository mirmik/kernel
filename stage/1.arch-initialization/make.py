#!/usr/bin/env python3

import licant
import os

licant.include("libc", "../../src/libc/libc.g.py")

licant.cxx_application("firmware.elf",
	toolchain=licant.cxx_make.toolchain_gcc("arm-none-eabi-"),

	sources=[
		"main.c",
		"../../src/arch/arm/stm32/asm/stm32_start.c"
	],
	
	include_paths=[
		"../../src/arch/arm/stm32/",
		"../../src/arch/arm/armv7-e/",
		"../../src/",
		"../../src/posix",
		"../../src/libc"
	],
	
	cc_flags="-mthumb -mcpu=cortex-m4",
	ld_flags=" ".join(["-nostdlib -nostdinc",
		"-T../../src/arch/arm/stm32/ldscripts/stm32f401re.ld",
		"-T../../src/arch/arm/stm32/ldscripts/stm32_common.ld"
	]),

	mdepends=["compat.libc"]
)

@licant.routine(deps=["firmware.elf"])
def install():
	os.system("arm-none-eabi-objcopy -O binary firmware.elf firmware.bin")
	os.system("st-flash write firmware.bin 0x8000000")

def terminal(port="/dev/ttyACM0"):
	os.system(f"gtkterm -p {port} -s 115200 --parity none")

licant.ex("firmware.elf")