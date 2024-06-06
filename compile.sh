sudo umount osfiles
dd if=/dev/zero of=test.img bs=1M count=100
sudo mkfs.fat -F32 test.img -s 1
sudo mount test.img osfiles
sudo cp -R content/. osfiles/

./i686-elf-as -W src/boot.s -o bin/boot.bin
gcc -O3 -m32 -c src/kernel.c -o bin/kernel.bin -ffreestanding
ld -m elf_i386 -T link.ld -o grub/boot/osbuild.bin bin/boot.bin bin/kernel.bin
sudo umount osfiles
grub-mkrescue -o bin/osbuild.iso grub/
