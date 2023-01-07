#!/bin/python
import sys
import os
from pathlib import Path

faustExtension = ".dsp"
targetExtension = ".hpp"

vaeFolder = os.path.realpath(os.path.dirname(os.path.realpath(__file__)) + os.sep + "..")
dspFolder = os.path.join(vaeFolder, "src", "core", "dsp")
outFolder = os.path.join(dspFolder, "generated")
archFile = os.path.join(dspFolder, "vae_faust_arch.hpp")

extras =  " --no-virtual"			# when compiled with the C++ backend, does not add the 'virtual' keyword.
extras += " -exp10"					# pow(10,x) replaced by possibly faster exp10(x).
extras += " -single"				# single precision float
extras += " --math-approximation"	# simpler/faster versions of 'floor/ceil/fmod/remainder' functions.
# extras += " -fm faust/dsp/fastmath.cpp" # this blows up the binary size by like half a megabyte, and i haven't benchmarked it yet
# extras += " --namespace vae::core::effect" # the inlcudes are inside the namespace so this is useless
extras += " --compute-mix"			# mix in outputs buffers.
# extras += " -vec"					# generate easier to vectorize code.

baseCommand = f"faust -lang cpp -i {extras} -a {archFile} -scn FaustBase"

Path(outFolder).mkdir(parents=True, exist_ok=True)

faustSources = []

for root, dirs, files in os.walk(dspFolder):
	path = root.split(os.sep)
	for file in files:
		if (file.find(faustExtension) != -1):
			faustSources.append(root + os.sep + file)


for source in faustSources:
	name = Path(source).stem
	outFile = os.path.join(outFolder, f"{name}_gen{targetExtension}")
	command = f"{baseCommand} -o {outFile} -cn {name.title()} {source}"
	print(command)
	if os.system(command) == 0:
		pass #  success

