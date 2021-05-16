from pysofaconventions import *
import matplotlib.pyplot as plt
import scipy.signal
import soundfile as sf
import numpy as np
import math

path = "scripts/MRT02.sofa" # cartesian
path = "scripts/dtf_nh2.sofa" # spherical
path = "scripts/subject_003.sofa" # spherical
sofa = SOFAFile(path,'r')
outPath = "scripts/out/"

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


for i in range(0, positions):
	form = "{:.2f}"
	x = sourcePositions[i][0] # azimuth
	y = sourcePositions[i][1] # elevation
	z = sourcePositions[i][2] # radius/distance
	if coordinateType == "cartesian":
		radius = math.sqrt(x * x + y * y + z * z)
		# todo the rest

	fileName = form.format(x) + "_" + form.format(y) + "_" + form.format(z) + ".wav"
	ir = irs[i, :, :]
	ir = ir.swapaxes(-1,0)
	sf.write(outPath + fileName, ir, sampleRate)


