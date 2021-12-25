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
baseCommand = f"faust -lang cpp -i -single -exp10 -cm -vec -mapp -a {archFile} -scn FaustBase"

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
	command = f"{baseCommand} -o {outFile} -cn VAE{name.title()} {source}"
	print(command)
	if os.system(command) == 0:
		pass #  success

