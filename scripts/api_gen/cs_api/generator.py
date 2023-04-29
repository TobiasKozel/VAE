import os
import sys
from jinja2 import Template, Environment
sys.path.insert(1, os.path.join(sys.path[0], ".."))
from api_gen_util_vae import loadApiDescription

jinja_env = Environment(autoescape=False, optimized=False, trim_blocks=True, lstrip_blocks=True)

def firstUpper(value):
    return value[:1].upper() + value[1:]

jinja_env.filters["first_upper"] = firstUpper

json_description = os.path.dirname(os.path.realpath(__file__)) + "/../public_api.json"
out_file = os.path.dirname(os.path.realpath(__file__)) + "/../../../include/vae/vae_cs_api.cs"
tempalte_file = os.path.dirname(os.path.realpath(__file__)) + "/template.cs"

print(f"Loading {json_description}")
description = loadApiDescription(json_description)

with open(tempalte_file, "r") as f:
	template = jinja_env.from_string(f.read())


basic_types = ["bool", "int"]
for i in description.types:
	basic_types.append(i["alias"])

with open(out_file, "w") as f:
	template.stream(
		Enums=description.enums,
		Structs=description.structs,
		Types=description.types,
		BasicTypes=basic_types
	).dump(f)
