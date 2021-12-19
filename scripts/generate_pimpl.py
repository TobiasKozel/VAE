# @addtogroup vae_scripts

import os
import sys
token = "_VAE_PUBLIC_API"
className = "EnginePimpl"
engineFile = os.path.dirname(os.path.realpath(__file__)) + "/../src/core/vae_engine.hpp"
pimplHeader = os.path.dirname(os.path.realpath(__file__)) + "/../include/vae/vae_pimpl.hpp"
pimplSource = os.path.dirname(os.path.realpath(__file__)) + "/../src/api/vae_pimpl.cpp"

class Param:
	def __init__(self):
		self.name = ""
		self.typename = ""
		self.default = False

class Func:
	def __init__(self):
		self.name = ""
		self.returns = ""
		self.text = ""
		self.parameters = []
		self.docblock = []

functions = []


#####
##### Parse core::Engine
#####

file = open(engineFile, "r")

lines = file.readlines()

for i in range(len(lines)):
	line = lines[i]
	func = Func()
	if line.find(token) == -1 or line.find("#define") != -1:
		continue
	if line.find("Engine") != -1:
		func.returns = "Engine"
		continue #ignore constructor for now

	for j in range(i, i + 5):
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
		functions.append(func)
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

	functions.append(func)

file.close()

#####
##### Generate header file
#####

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
	static %s* _VAE_API_EXPORT create(const EngineConfig& config);

	void _VAE_API_EXPORT destroy();

"""%((className,) * 7))

for func in functions:
	text = ""
	for doc in func.docblock:
		text += f"\t{doc}"

	text += f"\t{func.returns} _VAE_API_EXPORT {func.name} ("
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
"""}; // class %s

} // namespace vae
#endif // _VAE_GEN_PIMPL
"""%(className,))
file.close()

#####
##### Generate source file
#####

file = open(pimplSource, "w")

file.write("""
#include "../../include/vae/vae_pimpl.hpp"
#include "../core/vae_engine.hpp"
using namespace vae;
%s* %s::create() {
	auto e = new core::Engine();
	return reinterpret_cast<%s*>(e);
}

%s* %s::create(const EngineConfig& config) {
	auto e = new core::Engine(config);
	return reinterpret_cast<%s*>(e);
}

void %s::destroy() {
	auto e = reinterpret_cast<core::Engine*>(this);
	delete e;
}

"""%((className,) * 7))

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
