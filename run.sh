#!/bin/bash

qemu-system-x86_64 -rtc base=2011-09-09T09:09:00 --enable-kvm -drive file=test.img,format=raw -boot d --cdrom bin/osbuild.iso
