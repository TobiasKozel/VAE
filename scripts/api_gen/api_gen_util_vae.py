#!/bin/python
import json


class Enum:
	def __init__(self):
		self.type = "Enum"
		self.name = ""
		self.children = []

class EnumValue:
	def __init__(self):
		self.type = "EnumValue"
		self.name = ""
		self.index = 0

class Property:
	def __init__(self):
		self.type = "Property"
		self.typename = ""
		self.realtype = ""
		self.name = None
		self.default = None
		self.const = False
		self.static = False
		self.reference = False

	def fromString(self, line):
		if line.find("=") != -1:
			parts = line.split("=")
			self.default = parts[1].strip()
			line = parts[0].strip()
		else:
			self.default = None

		if line.find("const") != -1:
			self.const = True
			line = line.split("const")[1]

		line = line.strip()
		line = line.split(" ")
		self.name = line[-1]

		line[-1] = ""
		for k in line:
			self.typename = self.typename + k + " "

		self.typename = self.typename.strip()
		if self.typename.find("&") != -1:
			self.reference = True
			self.typename = self.typename.strip("&")


	def fromReturnString(self, line):
		if line.find("const") != -1:
			self.const = True
			line = line.split("const")[1]

		self.typename = line.strip()

class Func:
	def __init__(self):
		self.type = "Func"
		self.name = ""
		self.returns = Property()
		self.text = ""
		self.parameters = []
		self.docblock = []

class Struct:
	def __init__(self):
		self.type = "Struct"
		self.name = ""
		self.properties = []
		self.functions = []
		self.namespace = "vae"

class Type:
	def __init__(self):
		self.type = "Type"
		self.alias = ""
		self.typename = ""

# python really can be a heap of garbage
# https://stackoverflow.com/a/23689767
class dotdict(dict):
	# want to access dict with . so it's less of a mess when using it inside format strings
	__getattr__ = dict.get
	__setattr__ = dict.__setitem__
	__delattr__ = dict.__delitem__

def loadHook(dct):
	# todo too lazy to figure typed deserialization out
	return dotdict(dct)

def loadApiDescription(path):
	with open(path, "r") as infile:
		return json.load(infile, object_hook=loadHook)
