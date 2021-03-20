#!/bin/sh

echo "Updating rtaudio"
rm -rf ./rtaudio
rm ./master
wget https://codeload.github.com/thestk/rtaudio/zip/refs/heads/master
unzip ./master
rm ./master
mv ./rtaudio-master ./rtaudio

echo "Updaint httplib"
rm ./httplib.h
wget https://raw.githubusercontent.com/yhirose/cpp-httplib/master/httplib.h

