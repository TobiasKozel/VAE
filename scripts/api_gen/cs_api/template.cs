using System;
using System.Runtime.InteropServices;

namespace vae {
	static class vae_c_api
	{
		#if DEBUG
			public const string dll = "vae_c_apid.dll";
		#else
			public const string dll = "vae_c_api.dll";
		#endif
	}

{% for en in Enums %}
	enum {{ en.name }} {
	{% for value in en.children %}
		{{ value.name }} = {{ value.index}}{{ "," if not loop.last else "" }}
	{% endfor %}
	}
{% endfor %}

{% for t in Types %}
	{% if t.typename == "unsigned int" %}
	using {{ t.alias }} = System.UInt32;
	{% elif t.typename == "float" %}
	using {{ t.alias }} = System.Single;
	{% elif t.typename == "double" %}
	using {{ t.alias }} = System.Double;
	{% elif t.typename == "unsigned char" %}
	using {{ t.alias }} = System.Byte;
	{% elif t.typename == "unsigned short" %}
	using {{ t.alias }} = System.UInt16;
	{% elif t.typename == "void*" %}
	using {{ t.alias }} = System.IntPtr;
	{% elif t.typename == "const char*" %}
	using {{ t.alias }} = string;
	{% else %}
	// Missing type conversion for {{ t.alias }}
	// https://docs.microsoft.com/en-us/dotnet/csharp/language-reference/builtin-types/integral-numeric-types
	{% endif %}
{% endfor %}

{% for structure in Structs %}
	public class {{ structure.name }} : IDisposable {
		public Pointer _ptr;
		public bool _owned = false;

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Pointer vae_create_{{ structure.name }}();
		public {{ structure.name }}() {
			_ptr = vae_create_{{ structure.name }}();
			_owned = true;
		}

		// Temporary reference object
		public {{ structure.name }}(Pointer p) {
			_ptr = p;
			_owned = false;
		}

		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_destroy_{{ structure.name }}(Pointer obj);
		public void Dispose() {
			if (_owned) { vae_destroy_{{ structure.name }}(ptr); }
			Dispose();
			GC.SuppressFinalize(this);
		}

	{% for prop in structure.properties %}
		{% if prop.typename in BasicTypes %}
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern {{ prop.typename }} vae_{{ structure.name }}_get_{{ prop.name }}(Pointer obj);
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_{{ structure.name }}_set_{{ prop.name }}(Pointer obj, {{ prop.typename }} value);
		public {{ prop.typename }} {{ prop.name }} {
			get => vae_{{ structure.name }}_get_{{ prop.name }}(_ptr);
			set => vae_{{ structure.name }}_set_{{ prop.name }}(_ptr, value);
		}
		{% else %}
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Pointer vae_{{ structure.name }}_get_{{ prop.name }}(Pointer obj);
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_{{ structure.name }}_set_{{ prop.name }}(Pointer obj, Pointer value);
		public {{ prop.typename }} {{ prop.name }} {
			get => new {{ prop.typename }}(vae_{{ structure.name }}_get_{{ prop.name }}(_ptr));
			set => vae_{{ structure.name }}_set_{{ prop.name }}(_ptr, value._ptr);
		}
		{% endif %}

	{% endfor %}

	{% for function in structure.functions %}
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		{{ function.returns.typename }} vae_{{ structure.name }}_{{ function.name }}(
			Pointer obj{% for parameter in function.parameters %},
			{% if parameter.typename in BasicTypes %}
			{{ parameter.typename}} {{ parameter.name }}{% else %}
			Pointer {{ parameter.name }}{% endif %}
			{% endfor %}

		);

		{{ function.returns.typename }} {{ function.name }}(
			{% for parameter in function.parameters %}
			{% if parameter.typename in BasicTypes %}
			{{ parameter.typename}} {{ parameter.name }}{% else %}
			Pointer {{ parameter.name }}{% endif %}{{ "," if not loop.last else "" }}
			{% endfor %}
		) {
			return vae_{{ structure.name }}_{{ function.name }}(
				_ptr{% for parameter in function.parameters %},
				{% if parameter.typename in BasicTypes %}
				{{ parameter.name }}{% else %}
				{{ parameter.name }}._ptr{% endif %}
			{% endfor %}

			);
		}

	{% endfor %}
	}

{% endfor %}
}
