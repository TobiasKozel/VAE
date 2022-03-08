#include "../core/vae_engine.hpp"
// #include "../../../src/core/vae_engine.hpp"

#if defined(_WIN32) || defined(__CYGWIN__)
	#define _VAE_API_EXPORT __declspec(dllexport)
#else
	#define _VAE_API_EXPORT __attribute__((visibility("default")))
#endif

using namespace vae;
using namespace core;

extern "C"
{
{% for structure in Structs %}
	////////////// {{ structure.name }}
	_VAE_API_EXPORT Pointer vae_create_{{ structure.name }}() {
		return new {{ structure.name }}();
	}

	_VAE_API_EXPORT void vae_destroy_{{ structure.name }}(Pointer obj) {
		delete reinterpret_cast<{{ structure.name }}*>(obj);
	}

	{% for property in structure.properties %}
		///// {{ structure.name }}.{{ property.name }}
	{% if property.typename in BasicTypes %}
		_VAE_API_EXPORT void vae_{{ structure.name }}_set_{{ property.name }}(Pointer obj, {{ property.typename }} value) {
			reinterpret_cast<{{ structure.name }}*>(obj)->{{ property.name }} = value;
		}
		_VAE_API_EXPORT {{ property.typename }} vae_{{ structure.name }}_get_{{ property.name }}(Pointer obj) {
			return reinterpret_cast<{{ structure.name }}*>(obj)->{{ property.name }};
		}
	{% else %}
		_VAE_API_EXPORT void vae_{{ structure.name }}_set_{{ property.name }}(Pointer obj, Pointer value) {
			reinterpret_cast<{{ structure.name }}*>(obj)->{{ property.name }} =
				*reinterpret_cast<{{ property.typename }}*>(value);
		}
		_VAE_API_EXPORT Pointer vae_{{ structure.name }}_get_{{ property.name }}(Pointer obj) {
			return &reinterpret_cast<{{ structure.name }}*>(obj)->{{ property.name }};
		}
	{% endif %}
	{% endfor %}

	{% for function in structure.functions %}
		///// {{ structure.name }}.{{ function.name }}
		_VAE_API_EXPORT {{ function.returns.typename }} vae_{{ structure.name }}_{{ function.name }}(
			Pointer obj{% for parameter in function.parameters %},
			{% if parameter.typename in BasicTypes %}
			{{ parameter.typename}} {{ parameter.name }}{% else %}
			Pointer {{ parameter.name }}{% endif %}
			{% endfor %}

		) {
			return reinterpret_cast<{{ structure.name }}*>(obj)->{{ function.name }}(
			{% for parameter in function.parameters %}
				{% if parameter.typename in BasicTypes %}
				{{ parameter.name }}{% else %}
				*reinterpret_cast<{{ parameter.typename}}*>({{ parameter.name }}){% endif %}{{ "," if not loop.last else "" }}
			{% endfor %}
			);
		}

	{% endfor %}
{% endfor %}

} // extern "C"
