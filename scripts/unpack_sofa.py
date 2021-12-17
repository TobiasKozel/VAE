#!/usr/bin/python

# Small script to decode sofa files and write them into a simpler format

from pysofaconventions import *
import os
import numpy as np
import msgpack

vaeFolder = os.path.dirname(os.path.realpath(__file__)) + "/.."

path = vaeFolder + "/scripts/MRT01.sofa" # cartesian
# path = vaeFolder + "/scripts/dtf_nh2.sofa" # spherical
# path = vaeFolder + "/scripts/subject_003.sofa" # spherical

sofa = SOFAFile(path, "r")
outPath = vaeFolder + "/dev/hrtf.msgpack"

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

outData = { }
outData["samplerate"] = sampleRate
outData["positions"] = []

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
	outData["positions"].append(position)


with open(outPath, "wb") as outfile:
	packed = msgpack.packb(outData)
	outfile.write(packed)
