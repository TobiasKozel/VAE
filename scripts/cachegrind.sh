#!/bin/sh
cd ../build/dev
valgrind --tool=cachegrind --log-fd=1 ./vae_dev
