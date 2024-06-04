cd i686-elf/bin

./i686-elf-as -W ../../src/boot.s -o ../../bin/boot.bin
./i686-elf-gcc -O3 -std=c99 -m32 -c ../../src/kernel.c -o ../../bin/kernel.bin -ffreestanding
./i686-elf-ld -m elf_i386 -T ../../link.ld -o ../../bin/osbuild.bin ../../bin/boot.bin ../../bin/kernel.bin
