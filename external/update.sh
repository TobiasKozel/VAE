#!/bin/sh

echo "updating portaudio"
rm ./portaudio
rm ./master
curl -LO https://codeload.github.com/Be-ing/portaudio/zip/refs/heads/cmake_rewrite
unzip ./master > /dev/null 2>&1
mv ./portaudio-cmake_rewrite ./portaudio

# echo "Updating rtaudio"
# rm -rf ./rtaudio
# rm ./master
# curl -LO https://codeload.github.com/thestk/rtaudio/zip/refs/heads/master
# unzip ./master > /dev/null 2>&1
# rm ./master
# mv ./rtaudio-master ./rtaudio

echo "Updating httplib"
rm ./httplib.h
curl -LO https://raw.githubusercontent.com/yhirose/cpp-httplib/master/httplib.h

exit
echo "Updating flecs"
rm -rf ./flecs
rm ./master
curl -LO https://codeload.github.com/SanderMertens/flecs/zip/refs/heads/master
unzip ./master > /dev/null 2>&1
rm ./master
mv ./flecs-master ./flecs

