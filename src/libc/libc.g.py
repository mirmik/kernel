import licant

licant.module("compat.libc", 
	include_paths = ["include", "include/libc/", "include_internal"],
	sources = ["stdlib/*.c", "string/*.c", "ctype/*.c", "stdio/*.c", "math/*.c"]
)


