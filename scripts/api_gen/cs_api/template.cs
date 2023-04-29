using System;
using System.Runtime.InteropServices;

namespace vae {
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
	using {{ t.alias }} = System.String;
	{% else %}
	// Missing type conversion for {{ t.alias }}
	// https://docs.microsoft.com/en-us/dotnet/csharp/language-reference/builtin-types/integral-numeric-types
	{% endif %}
{% endfor %}

	static class vae_c_api
	{
		public const string dll = "vae_c_api";
	}

{% for en in Enums %}
	public enum {{ en.name }} {
	{% for value in en.children %}
		{{ value.name }} = {{ value.index}}{{ "," if not loop.last else "" }}
	{% endfor %}
	}
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
			if (_owned) { vae_destroy_{{ structure.name }}(_ptr); }
			Dispose();
			GC.SuppressFinalize(this);
		}

		// ------------------------------------------
		// 			PROPERTIES
		// ------------------------------------------
	{% for prop in structure.properties %}
		// property {{ structure.name }} {{ prop.name }}
		{% if prop.typename in BasicTypes %}
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern {{ prop.typename }} vae_{{ structure.name }}_get_{{ prop.name }}(Pointer obj);
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_{{ structure.name }}_set_{{ prop.name }}(Pointer obj, {{ prop.typename }} value);
		public {{ prop.typename }} {{ prop.name | first_upper }} {
			get => vae_{{ structure.name }}_get_{{ prop.name }}(_ptr);
			set => vae_{{ structure.name }}_set_{{ prop.name }}(_ptr, value);
		}
		{% else %}
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern Pointer vae_{{ structure.name }}_get_{{ prop.name }}(Pointer obj);
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void vae_{{ structure.name }}_set_{{ prop.name }}(Pointer obj, Pointer value);
		public {{ prop.typename }} {{ prop.name | first_upper }} {
			get => new {{ prop.typename }}(vae_{{ structure.name }}_get_{{ prop.name }}(_ptr));
			set => vae_{{ structure.name }}_set_{{ prop.name }}(_ptr, value._ptr);
		}
		{% endif %}

	{% endfor %}

		// ------------------------------------------
		// 			FUNCTIONS
		// ------------------------------------------
	{% for function in structure.functions %}
		[DllImport(vae_c_api.dll, CallingConvention = CallingConvention.Cdecl)]
		internal static extern {{ function.returns.typename }} vae_{{ structure.name }}_{{ function.name }}(
			Pointer obj{% for parameter in function.parameters %},
			{% if parameter.typename in BasicTypes %}
			{{ parameter.typename }} {{ parameter.name | first_upper }}{% else %}
			Pointer {{ parameter.name | first_upper }}{% endif %}
			{% endfor %}

		);

		public {{ function.returns.typename }} {{ function.name | first_upper }}(
			{% for parameter in function.parameters %}
			{{ parameter.typename }} {{ parameter.name | first_upper }}{{ "," if not loop.last else "" }}
			{% endfor %}
		) {
			{% if function.returns.typename != "void" %}
			return
			{% endif %}
			vae_{{ structure.name }}_{{ function.name }}(
				_ptr{% for parameter in function.parameters %},
				{% if parameter.typename in BasicTypes %}
				{{ parameter.name | first_upper }}{% else %}
				{{ parameter.name | first_upper }}._ptr{% endif %}
			{% endfor %});
		}

	{% endfor %}
	}

{% endfor %}
}
