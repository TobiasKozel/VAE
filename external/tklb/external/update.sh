#!/bin/sh

echo "Updating xsimd"
rm -rf ./xsimd
wget https://codeload.github.com/xtensor-stack/xsimd/zip/refs/heads/master
unzip ./master
rm ./master
mv ./xsimd-master ./xsimd
