import os
import sys
token = "_VAE_PUBLIC_API"
className = "EnginePimpl"
engineFile = os.path.dirname(os.path.realpath(__file__)) + "/../src/core/vae_engine.hpp"
pimplHeader = os.path.dirname(os.path.realpath(__file__)) + "/../include/vae/vae_pimpl.hpp"
pimplSource = os.path.dirname(os.path.realpath(__file__)) + "/../include/vae/vae_pimpl.cpp"

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

functions = []

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
	parts = func.text.split()
	func.returns = parts[1]
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

file = open(pimplHeader, "w")
file.write("#ifndef _VAE_GEN_PIMPL\n")
file.write("#define _VAE_GEN_PIMPL\n")
file.write("#include \"./vae.hpp\"\n\n")
file.write("namespace vae { \n")
file.write("class %s { \n"%className)

for func in functions:
	text = ""
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



file.write("}; // class %s\n"%className)
file.write("} // namespace vae\n")
file.write("#endif // _VAE_GEN_PIMPL\n")
file.close()

file = open(pimplSource, "w")
file.write("#include \"./vae_pimpl.hpp\"\n")
file.write("#include \"../../src/core/vae_engine.hpp\"\n\n")
file.write("using namespace vae;\n\n")
for func in functions:
	text = ""
	text += f"{func.returns} {className}::{func.name} ("
	for i in range(len(func.parameters)):
		param = func.parameters[i]
		text += f"\n\t\t{param.typename} {param.name}"
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

file.close()
