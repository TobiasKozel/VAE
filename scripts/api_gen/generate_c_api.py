#!/bin/python

import os
import sys
from api_gen_util_vae import loadApiDescription

className = "EnginePimpl"
jsonDescription = os.path.dirname(os.path.realpath(__file__)) + "/public_api.json"
apiFile = os.path.dirname(os.path.realpath(__file__)) + "/../../src/api/vae_c_api.cpp"

print(f"Loading {jsonDescription}")
desc = loadApiDescription(jsonDescription)

prefix = "vae"

#####
##### Generate file
#####

def isEnum(typename):
	for i in desc.enums:
		if typename == i.name:
			return True
	return False


def isBuiltIn(typename):
	for i in desc.structs:
		if i.name == typename:
			return False
	return True

def toBasicType(typename):
	if isEnum(typename):
		return "int"
	if isBuiltIn(typename):
		return typename
	return "void*"

functions = ""

for i in desc.structs:
	cast = f"reinterpret_cast<{i.namespace}::{i.name}*>(obj)"
	function = f"""
	void* {prefix}_create_{i.name}() {{
		return new {i.namespace}::{i.name}();
	}}

	void {prefix}_destroy_{i.name}(void* obj) {{
		delete {cast};
	}}
	"""

	for j in i.properties:
		typ = f"""{"const " if j.const else ""}{j.realtype}"""
		value = "value"
		pointer = ""
		if not isBuiltIn(j.realtype):
			value = f"""*reinterpret_cast<{i.namespace}::{typ}*>(value)"""
			typ = f"""{"const " if j.const else ""}void*"""
			pointer = "&"

		prop = f"""
	void {prefix}_{i.name}_set_{j.name}(void* obj, {typ} value) {{
		{cast}->{j.name} = {value};
	}}

	{typ} {prefix}_{i.name}_get_{j.name}(void* obj) {{
		return {pointer}{cast}->{j.name};
	}}
		"""
		function += prop


	for j in i.functions:
		returns = toBasicType(j.returns.realtype)
		returnCast = ""
		if isEnum(j.returns.realtype):
			returnCast = "(int) "
		if not isBuiltIn(j.returns.realtype):
			returns = f"""{"const " if j.returns.const else ""}{returns}"""

		params = "void* obj"
		forwardParams = ""
		for k in j.parameters:
			params += f",\n\t\t{toBasicType(k.realtype)} {k.name}"


		fun = f"""
	{returns} {prefix}_{i.name}_{j.name}(
		{params}
	) {{
		return {returnCast}{cast}->{j.name}({forwardParams});
	}}
"""
		function += fun

	functions += function


print(f"Writing {apiFile}")
file = open(apiFile, "w")
file.write(f"""
#include "../core/vae_engine.hpp"

extern "C"
{{
{functions}
}} // extern "C"
""")
file.close()
