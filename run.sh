#!/bin/bash

qemu-system-i386 --enable-kvm -drive file=test.img,format=raw -boot d --cdrom bin/osbuild.iso
