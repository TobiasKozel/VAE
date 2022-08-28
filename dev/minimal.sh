#!/bin/sh

g++ -Os -nodefaultlibs -nostartfiles -static -ffast-math -fno-stack-protector -fno-math-errno -fno-exceptions -fno-rtti -ffunction-sections -fdata-sections -Wl,--gc-sections ./minimal.cpp -o ./minimal

# g++ -Os -ffast-math -fno-stack-protector -fno-math-errno -fno-exceptions -fno-rtti -ffunction-sections -fdata-sections -Wl,--gc-sections ./minimal.cpp -o ./minimal
# strip -s -R .comment -R .gnu.version --strip-unneeded ./minimal
strip -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag ./minimal
