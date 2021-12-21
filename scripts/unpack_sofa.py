#!/usr/bin/python

# Small script to decode sofa files and write them into a simpler format

from pysofaconventions import *
import os
import numpy as np
import msgpack
import json

vaeFolder = os.path.dirname(os.path.realpath(__file__)) + "/.."

path = vaeFolder + "/scripts/MRT01.sofa" # cartesian
# path = vaeFolder + "/scripts/dtf_nh2.sofa" # spherical
# path = vaeFolder + "/scripts/subject_003.sofa" # spherical

sofa = SOFAFile(path, "r")
outPath = vaeFolder + "/../demo/banks/hrtf.json"

dimensions = sofa.getDimensionsAsDict()
positions = dimensions["M"].size
impulseLength = dimensions["N"].size
emitters = dimensions["E"].size # should be 1
receivers = dimensions["R"].size # should be 2, left/right

sourcePositions = sofa.getSourcePositionValues()
positionInfo = sofa.getSourcePositionInfo()
units = positionInfo[0]
coordinateType = positionInfo[1]
irs = sofa.getDataIR()

sampleRate = int(sofa.getSamplingRate().min())
if sampleRate != int(sofa.getSamplingRate().max()):
	print("mixed samplerates!")
	exit(1)

sampleRateUnit = sofa.getSamplingRateUnits() # should be hertz

listenerPos =  sofa.getListenerPositionValues() # should be 0, 0, 0

outData = { }
outData["samplerate"] = sampleRate
outData["positions"] = []

outData["up"] = [
	sofa.getListenerUpValues()[0][0],
	sofa.getListenerUpValues()[0][1],
	sofa.getListenerUpValues()[0][2]
]
outData["front"] = [
	sofa.getListenerViewValues()[0][0],
	sofa.getListenerViewValues()[0][1],
	sofa.getListenerViewValues()[0][2]
]

for i in range(0, positions):
	position = {}
	position["x"] = sourcePositions[i][0]
	position["y"] = sourcePositions[i][1]
	position["z"] = sourcePositions[i][2]
	if coordinateType == "cartesian":
		# radius = math.sqrt(x * x + y * y + z * z)
		# todo the rest
		pass

	ir = irs[i, :, :]
	ir = ir.swapaxes(-1,0)
	position["left"] = ir[:, 0].tolist()
	position["right"] = ir[:, 1].tolist()
	outData["positions"].append(position)


with open(outPath, "w") as outfile:
	json.dump(outData, outfile)
	# msgpack.pack(outData, outfile)
	# packed = msgpack.packb(outData)
	# outfile.write(packed)
