#!/bin/sh

echo "Updating rtaudio"
rm -rf ./rtaudio
rm ./master
wget https://codeload.github.com/thestk/rtaudio/zip/refs/heads/master
unzip ./master > /dev/null 2>&1
rm ./master
mv ./rtaudio-master ./rtaudio

echo "Updaint httplib"
rm ./httplib.h
wget https://raw.githubusercontent.com/yhirose/cpp-httplib/master/httplib.h

echo "Updating flecs"
rm -rf ./flecs
rm ./master
wget https://codeload.github.com/SanderMertens/flecs/zip/refs/heads/master
unzip ./master > /dev/null 2>&1
rm ./master
mv ./flecs-master ./flecs

