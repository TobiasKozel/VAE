#!/usr/bin/python

# Small script to decode sofa files and write them into a json

from pysofaconventions import *
import numpy as np
import json

path = "scripts/MRT01.sofa" # cartesian
# path = "scripts/dtf_nh2.sofa" # spherical
# path = "scripts/subject_003.sofa" # spherical
sofa = SOFAFile(path,'r')
outPath = "scripts/hrtf.json"

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

upVector = (sofa.getListenerUpValues()[0][0], sofa.getListenerUpValues()[0][1], sofa.getListenerUpValues()[0][2])
frontVector = (sofa.getListenerViewValues()[0][0], sofa.getListenerViewValues()[0][1], sofa.getListenerViewValues()[0][2])
listenerPos =  sofa.getListenerPositionValues() # should be 0, 0, 0

outJson = { }
outJson["samplerate"] = sampleRate
outJson["positions"] = []

for i in range(0, positions):
	position = {}
	position["x"] = round(sourcePositions[i][0], 4) # azimuth
	position["y"] = round(sourcePositions[i][1], 4) # elevation
	position["z"] = round(sourcePositions[i][2], 4) # radius/distance
	if coordinateType == "cartesian":
		# radius = math.sqrt(x * x + y * y + z * z)
		# todo the rest
		pass

	ir = irs[i, :, :]
	ir = ir.swapaxes(-1,0)
	ir *= (1 << 23) # to 24 bit
	ir = np.ma.round(ir).astype(int)
	position["left"] = ir[:, 0].tolist()
	position["right"] = ir[:, 1].tolist()
	outJson["positions"].append(position)


with open(outPath, "w") as outfile:
	json.dump(outJson, outfile, separators=(',', ':'))
