#!/bin/python

# Generates a cpp pimpl wrapper to seal of the header only madness

import os
import sys
from api_gen_util_vae import loadApiDescription

className = "EnginePimpl"
jsonDescription = os.path.dirname(os.path.realpath(__file__)) + "/public_api.json"
apiFile = os.path.dirname(os.path.realpath(__file__)) + "/../../src/api/vae_c_api.cpp"

print(f"Loading {jsonDescription}")
functions = loadApiDescription(jsonDescription)


#####
##### Generate file
#####

print(f"Writing {apiFile}")
file = open(apiFile, "w")
file.write("""
#include "../core/vae_engine.hpp"

extern "C"
{
	void* vae_create_engine() {
		return new vae::core::Engine();
	}



	void vae_destroy_engine(void* engine) {
		delete (vae::core::Engine*) engine;
	}


""")

for func in functions:
	text = ""
	for doc in func.docblock:
		text += f"\t{doc}"

	text += f"\t{func.returns} {func.name} ("
	for i in range(len(func.parameters)):
		param = func.parameters[i]
		text += f"\n\t\t{param.typename} {param.name}"
		if (param.default):
			text += f" = {param.default}"
		if (i < len(func.parameters) - 1):
			text += ","
		else:
			text += "\n\t"
	text += ");\n\n"
	file.write(text)

file.write(
"""
} // extern "C"
""")

file.close()
