# VAE /scripts/
Contains some handy scripts to generate ressources for development with VAE

# compile_faust.py
Traverses the src/core/dsp folder and compiles all faust source files.

Result will be stored src/core/dsp/generated

# generate_bank_defines.py
Will generate c++ headers from bank files which can be used to get autocompletion when calling vae::EnginePimpl::fireGlobalEvent

Usage:

```generate_bank_defines.py /path/to/rootfolder/ /path/to/result.hpp```

# unpack_sofa.py
Uses pysofaconventions to open up sofa HRTF files and decode them to a simpler format so we don't need to bring in a new depency for loading HRTF files.

# generate_pimpl.py
Reads vae::core::Engine and extracts all functions marked with _VAE_PUBLIC_API and generates
a the vae::EnginePimpl class.
