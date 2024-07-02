#!/bin/bash

# specify your project directory here
DIR=/home/strokeloh/Projects/pendos

sudo umount $DIR/osfiles
dd if=/dev/zero of=$DIR/test.img bs=1M count=100
sudo mkfs.fat -F32 $DIR/test.img -s 1
sudo rm -rf $DIR/content/name
sudo rm -rf $DIR/content/pc
sudo echo -n user >> $DIR/content/name
sudo echo -n pendos >> $DIR/content/pc
sudo mount $DIR/test.img $DIR/osfiles
cp -R $DIR/content/* $DIR/osfiles/

$DIR/i686-elf-as -W $DIR/src/boot.s -o $DIR/bin/boot.bin
gcc -O2 -m32 -c $DIR/src/kernel.c -o $DIR/bin/kernel.bin -ffreestanding
ld -m elf_i386 -T link.ld -o $DIR/grub/boot/osbuild.bin $DIR/bin/boot.bin $DIR/bin/kernel.bin
grub-mkrescue -o $DIR/bin/osbuild.iso $DIR/grub/
sudo umount $DIR/osfiles
