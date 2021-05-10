# -*- coding: utf-8 -*-

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#
#   plotListenHRTF.py
#
#   Example for getting HRTF data from a sofa file
#   Plots the HRTF and convolves to obtain binaural sound
#
#   (C) Andrés Pérez-López - Eurecat / UPF
#   30/08/2018
#
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

from pysofaconventions import *
import matplotlib.pyplot as plt
import scipy.signal
import soundfile as sf
import numpy as np

# Let's use subject_003 from the classical CIPIC database
# http://sofacoustics.org/data/database/cipic/subject_003.sofa
path = 'scripts/MRT02.sofa'
path = 'scripts/dtf_nh2.sofa'
path = 'scripts/subject_003.sofa'
sofa = SOFAFile(path,'r')

print("\n")
print("File is valid:", sofa.isValid())



# Let's see the dimensions:
#   - M: 1250 (different measurement positions)
#   - R: 2 (the two ears)
#   - E: 1 (one loudspeaker)
#   - N: 200 (lenght of the HRTFs in samples)
print("\n")
print("Dimensions:")
sofa.printSOFADimensions()


# Let's check the position of the measurementa (Source position)
sourcePositions = sofa.getVariableValue('SourcePosition')
print("\n")
print("Source Positions")
print(sourcePositions)
# and the info (units, coordinates)
print(sofa.getPositionVariableInfo('SourcePosition'))

# Let's inspect the first measurement
m = 0
print("\n")
print("Source Position of measurement " + str(m))
print(sourcePositions[m])
# which is at 82 degrees azimuth, -7 degrees elevation

# Read the data
data = sofa.getDataIR()
# and get the HRTF associated with m=0
hrtf = data[m,:,:]

# Let's check the dimensions of the hrtf
print("\n")
print("HRTF dimensions")
print(hrtf.shape)

# It looks fine, so let's plot it
# plt.plot(hrtf[0], label="left", linewidth=0.5,  marker='o', markersize=1)
# plt.plot(hrtf[1], label="right", linewidth=0.5,  marker='o', markersize=1)
# plt.grid()
# plt.legend()
# plt.show()
# It's pretty clear, based on the ITD and ILD, that the source is located at the left,
# which on the other hand confirms the sourcePositions[0] information


# Let's render it with a file and listen to it
# Open a mono wav file. I got this one from freesound
# https://freesound.org/people/Ryntjie/sounds/365061/
data, samplerate = sf.read('scripts/in.wav')

# Convolve it with the hrtf
binaural_left = scipy.signal.fftconvolve(data,hrtf[0])
binaural_right = scipy.signal.fftconvolve(data,hrtf[1])
binaural = np.asarray([binaural_left, binaural_right]).swapaxes(-1,0)

# Write to a file, and enjoy!
sf.write('scripts/binaural.wav', binaural, samplerate)
