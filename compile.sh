#!/bin/bash

sudo umount osfiles
dd if=/dev/zero of=test.img bs=1M count=100
sudo mkfs.fat -F32 test.img -s 1
sudo rm -rf content/name
sudo rm -rf content/pc
sudo echo -n user >> content/name
sudo echo -n pendos >> content/pc
sudo mount test.img osfiles
cp -R content/* osfiles/

./i686-elf-as -W src/boot.s -o bin/boot.bin
gcc -O2 -m32 -c src/kernel.c -o bin/kernel.bin -ffreestanding
ld -m elf_i386 -T link.ld -o grub/boot/osbuild.bin bin/boot.bin bin/kernel.bin
grub-mkrescue -o bin/osbuild.iso grub/
sudo umount osfiles
