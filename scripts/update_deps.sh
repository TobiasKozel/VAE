#!/bin/sh

cd "$(dirname "$0")/../external"

echo "Updating portaudio"
curl -LO https://github.com/PortAudio/portaudio/archive/refs/heads/master.zip
unzip ./master.zip > /dev/null 2>&1
rm ./master.zip
rm -rf ./portaudio
mv ./portaudio-master ./portaudio

echo "Updating rtaudio"
curl -LO https://github.com/thestk/rtaudio/archive/refs/heads/master.zip
unzip ./master.zip > /dev/null 2>&1
rm ./master.zip
rm -rf ./rtaudio
mv ./rtaudio-master ./rtaudio

echo "Updating glm"
curl -LO https://github.com/g-truc/glm/archive/refs/heads/master.zip
unzip ./master.zip > /dev/null 2>&1
rm ./master.zip
rm -rf ./glm
mv ./glm-master ./glm

echo "Updating tracey"
curl -LO https://github.com/wolfpld/tracy/archive/refs/tags/v0.7.8.zip
unzip ./v0.7.8.zip > /dev/null 2>&1
rm ./v0.7.8.zip
rm -rf ./tracy
mv ./tracy-0.7.8 ./tracy

cd headeronly

echo "Updating httplib"
rm ./httplib.h
curl -LO https://raw.githubusercontent.com/yhirose/cpp-httplib/master/httplib.h


echo "Updating json"
rm ./json.c
rm ./json.h
curl -LO https://raw.githubusercontent.com/json-parser/json-parser/master/json.c
curl -LO https://raw.githubusercontent.com/json-parser/json-parser/master/json.h

echo "Updating hash map"
rm ./robin_hood.h
curl -LO https://raw.githubusercontent.com/martinus/robin-hood-hashing/master/src/include/robin_hood.h

cd ..

