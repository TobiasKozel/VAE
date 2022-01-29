#!/bin/python

import os
import sys
import json
from api_gen_util_vae import *

# Small script to analyze the c++ api code and write it to a json

token = "_VAE_PUBLIC_API"
engineFile = os.path.dirname(os.path.realpath(__file__)) + "/../../src/core/vae_engine.hpp"
enumFile = os.path.dirname(os.path.realpath(__file__)) + "/../../include/vae/vae_enums.hpp"
structFile = os.path.dirname(os.path.realpath(__file__)) + "/../../include/vae/vae_structs.hpp"

jsonDescription = os.path.dirname(os.path.realpath(__file__)) + "/public_api.json"

enums = []
structs = []

#####
##### Parse enums
#####

print(f"Opening enums {enumFile}")
file = open(enumFile, "r")
lines = file.readlines()

for i in range(len(lines)):
	line = lines[i]

	if (line.find("enum class") == -1):
		continue # look for enum start

	enum = Enum()
	enum.name = line.split(" ")[2]

	j = i + 1
	while (1):
		line = lines[j]
		if line.find("};") != -1:
			break
		enumValue = EnumValue()
		enumValue.name = line.split(",")[0]
		enumValue.name = line.split("//")[0]
		enumValue.name = enumValue.name.split("=")[0]
		enumValue.name = enumValue.name.strip()
		enum.values.append(enumValue)
		j += 1

	enums.append(enum)
	i = j + 1

#####
##### Parse public structs
#####

print(f"Opening structs {structFile}")
file = open(structFile, "r")
lines = file.readlines()

for i in range(len(lines)):
	line = lines[i].strip()
	line = line.split(" ")
	if (line[0] != "struct"):
		continue # look struct start

	struct = Struct()
	struct.name = line[1]

	j = i + 1
	while (1):
		line = lines[j]
		if line.find("};") != -1:
			break # end of struct

		if line.find(";") == -1:
			j += 1
			continue
		# found property

		if line.find("EventCallbackData") != -1:
			j += 1
			continue # skip function pointer

		prop = Property()

		line = line.split(";")[0]

		if line.find("=") != -1:
			parts = line.split("=")
			prop.default = parts[1].strip()
			line = parts[0].strip()
		else:
			prop.default = None

		if line.find("const") != -1:
			prop.const = True
			line = line.split("const")[1]

		line = line.strip()
		line = line.split(" ")
		prop.name = line[-1]
		line[-1] = ""
		for k in line:
			prop.typename = prop.typename + k + " "

		prop.typename = prop.typename.strip()
		struct.properties.append(prop)
		j += 1


	structs.append(struct)
	i = j + 1

#####
##### Parse core::Engine
#####

engine = Struct()
engine.name = "Engine"
print(f"Opening core::Engine file {engineFile}")
file = open(engineFile, "r")
lines = file.readlines()

for i in range(len(lines)):
	line = lines[i]
	if line.find(token) == -1 or line.find("#define") != -1:
		continue # skip the defintion of the token itself

	func = Func()

	for j in range(i, i + 20):
			func.text += lines[j]
			if line.find(")") != -1:
				# function head end found
				break

	if (lines[i - 1].find("*/") != -1):
		# has docblock
		commentStartLine = i - 1
		# search the previous 20 line for block start
		while ((i - 20) < commentStartLine):
			l = lines[commentStartLine].replace("\t", "")
			func.docblock.append(l)
			if (l.find("/**") != -1):
				func.docblock.reverse()
				# found the start
				break
			commentStartLine -= 1

	parts = func.text.split()
	func.returns = parts[0]
	nameParts = parts[2].split("(")
	func.name = nameParts[0]
	if (nameParts[1] == ")"):
		engine.functions.append(func)
		continue # done
	parameterText = func.text.split("(")[1]
	parameterText = parameterText.split(")")[0]
	parameterPairs = parameterText.split(",")
	for p in parameterPairs:
		param = Param()
		p = p.strip()
		defaultSplit = p.split("=")
		if (len(defaultSplit) == 2):
			param.default = defaultSplit[1].strip()
		paramTypeSplit = defaultSplit[0].split()
		for j in range(len(paramTypeSplit) - 1):
			param.typename += " " + paramTypeSplit[j]
		param.typename = param.typename.strip()
		param.name = paramTypeSplit[len(paramTypeSplit) - 1]
		func.parameters.append(param)

	engine.functions.append(func)

file.close()
structs.append(engine)


#####
##### Write result
#####

print(f"Write description file to {jsonDescription}")

class CustomEncoder(json.JSONEncoder):
	def default(self, obj):
		return obj.__dict__

with open(jsonDescription, "w") as outfile:
	out = {
		"enums": enums,
		"structs": structs
	}
	json.dump(out, outfile, cls=CustomEncoder, indent=4)
