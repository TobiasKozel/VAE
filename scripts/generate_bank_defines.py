#!/usr/bin/python

# Small script to read all bank files and create a
# header file with all the events by name

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

banks = []

for root, dirs, files in os.walk(bankFolder):
	path = root.split(os.sep)
	for file in files:
		if (file.find("bank.json") != -1):
			with open(root + os.sep + file, "r") as file:
				decoded = json.load(file)
				banks.append(decoded)

def makeSane(name):
	return re.sub("[^a-zA-Z0-9]", "", name)

bankText = ""

for bank in banks:
	bankId = str(bank["id"])
	eventText = ""
	for event in bank["events"]:
		# Calculate the real global id at compile time
		# so we don't hardcode the size of vae::EventHandle here
		eventId = f"{str(event['id'])} | ({bankId} << (sizeof(vae::EventHandle) * 8))"
		eventText += f"\t\tstatic constexpr vae::GlobalEventHandle {makeSane(event['name'])} = {eventId};\n"

	bankText += f"""
	struct {makeSane(bank['name'])} {{
		static constexpr vae::BankHandle id = {bankId};
{eventText}
	}};
	"""

shell = f"""
#ifndef _VAE_GEN_EVENT_NAMES
#define _VAE_GEN_EVENT_NAMES
namespace vaeb {{
{bankText}
}} // namespace vaeb
#endif // _VAE_GEN_EVENT_NAMES
"""

with open(hppFile, "w") as file:
	file.write(shell)


