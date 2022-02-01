#!/bin/python

import os
import sys
from api_gen_util_vae import loadApiDescription

jsonDescription = os.path.dirname(os.path.realpath(__file__)) + "/public_api.json"
apiFile = os.path.dirname(os.path.realpath(__file__)) + "/../../include/vae/vae_cs_api.cs"
apiFile = "C:\\Users\\usr\\source\\repos\\vaec\\vae_cs_api.cs"

print(f"Loading {jsonDescription}")
desc = loadApiDescription(jsonDescription)

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
	if typename == "void*":
		return "IntPtr"
	if typename == "char*":
		return "string"
	if typename == "unsigned char":
		return "byte"
	if typename == "unsigned short":
		return "ushort"
	if typename == "unsigned int":
		return "uint"
	if typename == "float":
		return "float"
	if isEnum(typename):
		return "int"
	return "IntPtr"

def sanitizeVarName(name):
	if name == "event":
		return "@event"
	return name

prefix = "vae"
dllImport = "[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]"

def makeExternal(func):
	return f"""
		{dllImport}
		internal static extern {func};"""

def makeConstructor(clas):
	ctor = f"{prefix}_create_{clas}"
	dtor = f"{prefix}_destroy_{clas}"
	return f"""
		public IntPtr ptr;
		public bool owned = false;
		{makeExternal(f"IntPtr {ctor}()")}
		public {clas}() {{
			ptr = {ctor}();
			owned = true;
		}}

		public {clas}(IntPtr p) {{
			ptr = p;
			owned = false;
		}}

		{makeExternal(f"void {dtor}(IntPtr obj)")}
		public void Dispose() {{
			if (owned) {{ {dtor}(ptr); }}
			Dispose();
			GC.SuppressFinalize(this);
		}}
"""

def makeGetSet(typ, obj, member):
	ityp = toBasicType(typ)
	if isBuiltIn(typ):
		typ = ityp
	setter = f"{prefix}_{obj}_set_{member}"
	getter = f"{prefix}_{obj}_get_{member}"
	classSetter = f"{setter}(ptr, value)"
	classGetter = f"{getter}(ptr)"
	if ityp != typ:
		classSetter = f"{setter}(ptr, value.ptr)"
		classGetter = f"new {typ}({getter}(ptr))"
	return f"""
		{makeExternal(f"void {setter}(IntPtr obj, {ityp} value)")}
		{makeExternal(f"{ityp} {getter}(IntPtr obj)")}
		public {typ} {sanitizeVarName(member)} {{
			set {{ {classSetter}; }}
			get {{ return {classGetter}; }}
		}}"""

classes = ""

for i in desc.structs:
	funcs = makeConstructor(i.name)
	for j in i.properties:
		funcs += makeGetSet(j.realtype, i.name, j.name)

	for j in i.functions:
		cfunc = f"vae_{i.name}_{j.name}"
		retyp = toBasicType(j.returns.realtype)
		paramsForC = ""
		paramsForCs = ""
		forwardParams = ""
		isFirst = True
		for k in j.parameters:
			name = sanitizeVarName(k.name)
			paramsForC += f", {toBasicType(k.realtype)} {name}"
			if not isFirst:
				forwardParams += ","
				paramsForCs += ","
			isFirst = False
			if not isBuiltIn(k.typename):
				paramsForCs += f"\n\t\t\t{k.realtype} {name}"
				name = f"{name}.ptr"
			else:
				paramsForCs += f"\n\t\t\t{toBasicType(k.realtype)} {name}"

			forwardParams += f" {name}"

		if not isFirst:
			forwardParams = "," + forwardParams

		paramsForCs += "\n\t\t"

		funcs += f"""
		{makeExternal(f"{retyp} {cfunc}(IntPtr obj{paramsForC})")}
		public {retyp} {j.name}({paramsForCs}) {{
			return {cfunc}(ptr{forwardParams});
		}}"""

	clas = f"""
	public class {i.name} : IDisposable {{
		{funcs}
	}}"""
	classes += clas



print(f"Writing {apiFile}")
file = open(apiFile, "w")
file.write(f"""
using System;
using System.Runtime.InteropServices;

namespace vae {{
	static class vae_c_api
	{{
		#if DEBUG
			public const string dll = "vae_c_apid.dll";
		#else
			public const string dll = "vae_c_api.dll";
		#endif
	}}

	{classes}
}}

""")
file.close()
