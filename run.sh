#!/bin/bash

qemu-system-i386 -drive file=test.img,format=raw --enable-kvm -boot d --cdrom bin/osbuild.iso
