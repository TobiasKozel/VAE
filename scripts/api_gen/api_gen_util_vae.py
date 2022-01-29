#!/bin/python
import json

class Param:
	def __init__(self):
		self.type = "Param"
		self.name = ""
		self.typename = ""
		self.default = None

class Func:
	def __init__(self):
		self.type = "Func"
		self.name = ""
		self.returns = ""
		self.text = ""
		self.parameters = []
		self.docblock = []


class Enum:
	def __init__(self):
		self.type = "Enum"
		self.name = ""
		self.values = []

class EnumValue:
	def __init__(self):
		self.type = "EnumValue"
		self.name = ""

class Property:
	def __init__(self):
		self.type = "Property"
		self.typename = ""
		self.name = ""
		self.default = None
		self.const = False

class Struct:
	def __init__(self):
		self.type = "Struct"
		self.name = ""
		self.properties = []
		self.functions = []

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
