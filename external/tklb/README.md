# Defines

## General

### TKLB_NO_ASSERT
Will disable all assertions

### TKLB_NO_SIMD
Disables SSE or other intrinsics

### TKLB_MAXCHANNELS
Maximum channels for audio related classes. Defaults to 2

### TKLB_SAMPLE_FLOAT
Will do all audio related stuff with floats

## Memleak detection
### TKLB_LEAKCHECKER_NO_INFO
Disables additional info about allocations and the magic number at the end of each allocation to detect heap corruptions

### TKLB_MAGIC_NUMBER
Changes the magic number from above (normal integer)

## Convolver

### TKLB_CONVOLUTION_FLOAT
Will do float convolution which allows sse optimizations regardless of TKLB_SAMPLE_FLOAT
