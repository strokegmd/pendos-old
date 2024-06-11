#!/bin/bash

qemu-system-x86_64 -drive file=test.img,format=raw -boot d --cdrom bin/osbuild.iso
