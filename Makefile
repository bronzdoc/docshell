GCC=gcc

compile:
	$(GCC) -w -o dsh dsh.c lib/dsh_lib.c
compile_w:
	$(GCC) -o dsh dsh.c lib/dsh_lib.c


