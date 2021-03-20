#!/bin/sh

echo "Updating xsimd"
rm -rf ./xsimd
rm ./master
wget https://codeload.github.com/xtensor-stack/xsimd/zip/refs/heads/master
unzip ./master
rm ./master
mv ./xsimd-master ./xsimd

echo "Updating pffft"
rm -rf ./pffft
wget https://codeload.github.com/marton78/pffft/zip/refs/heads/master
unzip ./master
rm ./master
mv ./pffft-master ./pffft

echo "Updating speex_resampler"
rm -rf ./speex_resampler
wget https://codeload.github.com/xiph/speexdsp/zip/refs/heads/master
unzip ./master
rm ./master
mkdir ./speex_resampler
mv ./speexdsp-master/libspeexdsp/arch.h ./speex_resampler/arch.h
mv ./speexdsp-master/libspeexdsp/os_support.h ./speex_resampler/os_support.h
mv ./speexdsp-master/libspeexdsp/resample* ./speex_resampler/
mkdir ./speex_resampler/speex/
mv ./speexdsp-master/include/speex/speex_resampler.h ./speex_resampler/speex/speex_resampler.h
rm -rf ./speexdsp-master/
