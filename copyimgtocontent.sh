#!/bin/bash

sudo mount test.img osfiles
cp -R osfiles/* content/
sudo umount osfiles
