#!/usr/bin/python

import sys
import json
import os
import re

vaeFolder = os.path.dirname(os.path.realpath(__file__)) + os.sep + ".."
bankFolder = vaeFolder + os.sep + "dev"
hppFile = bankFolder + os.sep + "vae_def.hpp"

if len(sys.argv) == 3:
	bankFolder = sys.argv[1]
	hppFile = sys.argv[2]

bankFiles = []

for root, dirs, files in os.walk(bankFolder):
	path = root.split(os.sep)
	for file in files:
		if (file.find("bank.json") != -1):
			bankFiles.append(root + os.sep + file)

banks = []

for bankFile in bankFiles:
	with open(bankFile, "r") as file:
		decoded = json.load(file)
		bank = {
			"name": decoded["name"],
			"id": decoded["id"]
		}
		events = []
		for event in decoded["events"]:
			events.append(event)

		bank["events"] = events
		banks.append(bank)

def makeSane(name):
	return re.sub("[^a-zA-Z0-9]", "", name)

with open(hppFile, "w") as file:
	file.write("#ifndef _VAE_GEN_EVENT_NAMES\n")
	file.write("#define _VAE_GEN_EVENT_NAMES\n")
	file.write("namespace vaeb { \n")
	for bank in banks:
		bankId = str(bank["id"])
		file.write("struct %s {\n"%(makeSane(bank["name"],)))
		file.write("\tstatic constexpr vae::BankHandle id = %s;\n"%(bankId,))
		for event in events:
			# Calculate the real global id at compile time
			# so we don't hardcode the size of vae::EventHandle here
			eventId = "%s | (%s << (sizeof(vae::EventHandle) * 8))"%(str(event["id"]), bankId,)
			file.write("\tstatic constexpr vae::GlobalEventHandle %s = %s;\n"%(
				makeSane(event["name"]), eventId,
			))

		file.write("};\n")
	file.write("} // namespace vaeb\n")
	file.write("#endif // _VAE_GEN_EVENT_NAMES\n")
