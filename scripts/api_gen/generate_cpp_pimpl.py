#!/bin/python

import os
import sys
from api_gen_util_vae import loadApiDescription

className = "EnginePimpl"
jsonDescription = os.path.dirname(os.path.realpath(__file__)) + "/public_api.json"
pimplHeader = os.path.dirname(os.path.realpath(__file__)) + "/../../include/vae/vae_pimpl.hpp"
pimplSource = os.path.dirname(os.path.realpath(__file__)) + "/../../src/api/vae_pimpl.cpp"

print(f"Loading {jsonDescription}")
structs = loadApiDescription(jsonDescription).structs
functions = []

for i in structs:
	if i.name == "Engine":
		functions = i.functions


#####
##### Generate header file
#####

print(f"Writing {pimplHeader}")
file = open(pimplHeader, "w")
file.write("""
#ifndef _VAE_GEN_PIMPL
#define _VAE_GEN_PIMPL
#include "./vae.hpp"

#if defined(_WIN32) || defined(__CYGWIN__)
	#ifdef VAE_DLL_EXPORT
		#define _VAE_API_EXPORT __declspec(dllexport) __stdcall
	#else
		#define _VAE_API_EXPORT __stdcall
	#endif
#else
	#ifdef VAE_DLL_EXPORT
		#define _VAE_API_EXPORT __attribute__((visibility("default")))
	#else
		#define _VAE_API_EXPORT
	#endif
#endif

namespace vae {
/**
 * @brief Sealed of version of vae::core::Engine
 * @details There are a few function, which aren't exported
 * due to the fact that they would pull all the dependencies like
 * tklb::AudioBuffer back in.
 * @see vae::core::Engine
 */
class %s {
	%s();
	%s(const %s&);
	~%s() { };
public:
	static %s* _VAE_API_EXPORT create();

	void _VAE_API_EXPORT destroy();

"""%((className,) * 6))

for func in functions:
	text = ""
	for doc in func.docblock:
		text += f"\t{doc}"

	text += f"\t{func.returns} _VAE_API_EXPORT {func.name} ("
	for i in range(len(func.parameters)):
		param = func.parameters[i]
		text += f"\n\t\t{param.typename} {param.name}"
		if (param.default != None):
			text += f" = {param.default}"
		if (i < len(func.parameters) - 1):
			text += ","
		else:
			text += "\n\t"
	text += ");\n\n"
	file.write(text)

file.write(
"""}; // class %s

} // namespace vae
#endif // _VAE_GEN_PIMPL
"""%(className,))
file.close()

#####
##### Generate source file
#####

print(f"Writing {pimplSource}")
file = open(pimplSource, "w")

file.write("""
#include "../../include/vae/vae_pimpl.hpp"
#include "../core/vae_engine.hpp"
using namespace vae;
%s* %s::create() {
	auto e = new core::Engine();
	return reinterpret_cast<%s*>(e);
}

void %s::destroy() {
	auto e = reinterpret_cast<core::Engine*>(this);
	delete e;
}

"""%((className,) * 4))

for func in functions:
	text = ""
	text += f"{func.returns} {className}::{func.name} ("
	for i in range(len(func.parameters)):
		param = func.parameters[i]
		text += f"\n\t{param.typename} {param.name}"
		if (i < len(func.parameters) - 1):
			text += ","
		else:
			text += "\n"
	text += ") {\n"
	text += "\tauto& e = *reinterpret_cast<core::Engine*>(this);\n"
	text += f"\treturn e.{func.name}("
	for i in range(len(func.parameters)):
		param = func.parameters[i]
		text += f"\n\t\t{param.name}"
		if (i < len(func.parameters) - 1):
			text += ","
		else:
			text += "\n\t"
	text += ");\n}\n\n"
	file.write(text)

file.write("""
extern "C" %s* create() {
	return %s::create();
}
"""%((className,) * 2))

file.close()
