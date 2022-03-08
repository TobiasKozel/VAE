#!/bin/python

import os
import sys
import json
from api_gen_util_vae import *

# Primitive script to analyze the c++ api code and write it to a json

def abs_path(path):
	return os.path.dirname(os.path.realpath(__file__)) + path

token = "_VAE_PUBLIC_API"										# token to look for when exporting member functions
engineFile = abs_path("/../../src/core/vae_engine.hpp")			# only functions will be parsed
enumFile = abs_path("/../../include/vae/vae_enums.hpp")			# only enums will be parsed
structFile = abs_path("/../../include/vae/vae_structs.hpp")		# only members of structs will be parsed
typeDefsFile = abs_path("/../../include/vae/vae_type_defs.hpp")	# only type definitions/alias will be parsed
jsonDescription = abs_path("/public_api.json")					# result file

enums = []
structs = []
types = []

def resolve_type(name):
	for i in types:
		if i.alias == name:
			return i.typename

	return name

#####
##### Parse types
#####

print(f"Opening types {typeDefsFile}")
file = open(typeDefsFile, "r")
lines = file.readlines()

for i in range(len(lines)):
	line = lines[i]
	if (line.find("using") == -1):
		continue # look for type definition

	line = line.split("using ")[1]
	line = line.split(";")[0]
	parts = line.split("=")
	typ = Type()
	typ.alias = parts[0].strip()
	typ.typename = parts[1].strip()
	types.append(typ)


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
	index = 0
	while (1):
		line = lines[j]
		if line.find("};") != -1:
			break

		if index != 0 and line.find("=") != -1:
			print("Error: enum values not implemented")
			sys.exit()

		enumValue = EnumValue()
		enumValue.name = line.split(",")[0]
		enumValue.name = enumValue.name.split("//")[0]
		enumValue.name = enumValue.name.split("=")[0]
		enumValue.name = enumValue.name.strip()
		enumValue.index = index
		enum.children.append(enumValue)
		j += 1
		index += 1

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

		if line.find("[") != -1:
			j += 1
			continue # skip array types

		prop = Property()

		line = line.split(";")[0]
		prop.fromString(line)
		prop.realtype = resolve_type(prop.typename)
		struct.properties.append(prop)
		j += 1


	structs.append(struct)
	i = j + 1

#####
##### Parse core::Engine
#####

engine = Struct()
engine.namespace = "vae::core"
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
	func.returns = Property()
	func.returns.fromReturnString(parts[0])
	func.returns.realtype = resolve_type(func.returns.typename)

	nameParts = parts[2].split("(")
	func.name = nameParts[0]
	if (nameParts[1] == ")"):
		engine.functions.append(func)
		continue # done
	parameterText = func.text.split("(")[1]
	parameterText = parameterText.split(")")[0]
	parameterPairs = parameterText.split(",")
	for p in parameterPairs:
		param = Property()
		param.fromString(p)
		param.realtype = resolve_type(param.typename)
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
		"structs": structs,
		"types": types
	}
	json.dump(out, outfile, cls=CustomEncoder, indent=4)
