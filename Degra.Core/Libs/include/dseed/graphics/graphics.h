#ifndef __DSEED_GRAPHICS_H__
#define __DSEED_GRAPHICS_H__

namespace dseed::graphics
{
	struct DSEEDEXP displaymode
	{
		size2i resolution;
		color::pixelformat format;
		fraction refresh_rate;

		inline displaymode() noexcept = default;
		inline displaymode(const size2i& res, color::pixelformat fmt, fraction rr) noexcept
			: resolution(res), format(fmt), refresh_rate(rr)
		{ }
	};

	class DSEEDEXP display : public object, public wrapped
	{
	public:
		virtual error_t name(char* name, size_t maxNameCount) = 0;

	public:
		virtual error_t displaymode(int index, displaymode* mode) = 0;
		virtual size_t displaymode_count() = 0;

	public:
		virtual error_t area(rect2i* area) = 0;

	public:
		virtual error_t refresh() = 0;
	};

	class DSEEDEXP vgaadapter : public object, public wrapped
	{
	public:
		virtual error_t name(char* name, size_t maxNameCount) = 0;

	public:
		virtual error_t display(int index, display** display) = 0;
		virtual size_t display_count() = 0;

	public:
		virtual error_t refresh() = 0;
	};

	class DSEEDEXP vgaadapter_enumerator : public object
	{
	public:
		virtual error_t adapter(int index, vgaadapter** adapter) = 0;
		virtual size_t adapter_count() = 0;

	public:
		virtual error_t refresh() = 0;
	};
}

namespace dseed::graphics
{
	class DSEEDEXP texture : public object, public wrapped
	{
	public:
		virtual color::pixelformat format() noexcept = 0;
		virtual size_t mip_level() noexcept = 0;
	};

	class DSEEDEXP texture1d : public texture
	{
	public:
		virtual int32_t size() noexcept = 0;
	};

	class DSEEDEXP texture2d : public texture
	{
	public:
		virtual dseed::size2i size() noexcept = 0;
	};

	class DSEEDEXP texture3d : public texture
	{
	public:
		virtual dseed::size3i size() noexcept = 0;
	};

	class DSEEDEXP texturecube : public texture
	{
		virtual dseed::size2i size() noexcept = 0;
	};
}

namespace dseed::graphics
{
	enum class vgabuffertype
	{
		vertex,
		index,
		constant,
	};

	class DSEEDEXP vgabuffer : public object, public wrapped
	{
	public:
		virtual size_t stride() noexcept = 0;
		virtual size_t length() noexcept = 0;

	public:
		virtual vgabuffertype type() noexcept = 0;
	};
}

namespace dseed::graphics
{
	using shaderlang = const char*;

	constexpr const char* shaderlang_dxil = "DXIL";
	constexpr const char* shaderlang_hlsl = "HLSL";
	constexpr const char* shaderlang_hlsl_legacy = "HLSL_LEGACY";

	constexpr const char* shaderlang_spirv = "SPIR-V";
	constexpr const char* shaderlang_glsl = "GLSL";
	constexpr const char* shaderlang_glsl_legacy = "GLSL_LEGACY";
	constexpr const char* shaderlang_gles = "GLES";
	constexpr const char* shaderlang_gles_legacy = "GLES20";

	constexpr const char* shaderlang_metal = "METAL";

	constexpr const char* shaderlang_pssl = "PSSL";
	constexpr const char* shaderlang_psm = "PSM";

	constexpr const char* shaderlang_xbo = "XBO";

	enum class shadertype
	{
		// Basic Shaders
		vertex,
		pixel,

		// Geometry, Tessellation Shaders
		geometry,
		hull,
		domain,

		// Compute Shader
		compute,

		// AMD Primitive Shader
		primitive,

		// NVIDIA Mesh/Task Shaders
		task,
		mesh,
	};

	class DSEEDEXP shaderpack : public object
	{
	public:
		virtual shadertype type() noexcept = 0;
		virtual bool contains(shaderlang lang) noexcept = 0;

	public:
		virtual error_t at(shaderlang lang, blob** blob) noexcept = 0;
	};
}

namespace dseed::graphics
{
	enum class blendvalue
	{
		zero,
		one,
		src_color,
		inverted_src_color,
		src_alpha,
		inverted_src_alpha,
		dest_alpha,
		inverted_dest_alpha,
		dest_color,
		inverted_dest_color,
	};

	enum class blendop
	{
		add,
		subtract,
		reversed_subtract,
		min,
		max,
	};

	struct blendparams
	{
		bool blend_enable;
		blendvalue src;
		blendvalue dest;
		blendop blend_op;

		inline blendparams() noexcept = default;
		inline blendparams(bool blendenable, blendvalue src, blendvalue dest, blendop blendop) noexcept
			: blend_enable(blendenable), src(src), dest(dest), blend_op(blendop)
		{ }

		static inline blendparams noblend() noexcept { return blendparams(false, blendvalue::one, blendvalue::zero, blendop::add); }
		static inline blendparams alphablend() noexcept { return blendparams(true, blendvalue::src_alpha, blendvalue::inverted_src_alpha, blendop::add); }
		static inline blendparams additiveblend() noexcept { return blendparams(true, blendvalue::src_alpha, blendvalue::one, blendop::add); }
		static inline blendparams subtractblend() noexcept { return blendparams(true, blendvalue::src_alpha, blendvalue::one, blendop::reversed_subtract); }
		static inline blendparams multiplyblend() noexcept { return blendparams(true, blendvalue::dest_color, blendvalue::zero, blendop::add); }
	};

	enum class stencilop
	{
		keep,
		zero,
		invert,
		replace,
		increment,
		decrement,
	};

	struct depthstencilparams
	{
		bool depth_enable;
		bool stencil_enable;

		comparison_operator depth_func;

		comparison_operator stencil_func;
		uint8_t stencil_readmask;
		uint8_t stencil_writemask;
		stencilop stencil_pass;
		stencilop stencil_fail;
	};
}

namespace dseed::graphics
{
	class DSEEDEXP pipeline : public object, public wrapped
	{
	public:

	};
}

namespace dseed::graphics
{
	enum class rendermethod
	{
		forward,
		deferred,
	};

	class DSEEDEXP vgarender : public object, public wrapped
	{

	};
}

namespace dseed::graphics
{
	class DSEEDEXP vgadevice : public object, public wrapped
	{
	public:
		virtual error_t adapter(vgaadapter** adapter) noexcept = 0;

	public:
		virtual bool is_support_format(color::pixelformat pf) noexcept = 0;

	public:
		virtual error_t sprite_render(vgarender** render) noexcept = 0;

	public:
		virtual bool vsync() noexcept = 0;
		virtual error_t set_vsync(bool vsync) noexcept = 0;

	public:
		virtual void displaymode(displaymode* dm, bool* fullscreen) noexcept = 0;
		virtual bool set_displaymode(const graphics::displaymode* dm, bool fullscreen) noexcept = 0;

	public:
		virtual error_t present() noexcept = 0;
	};
}

namespace dseed::graphics
{
	enum class sprite_texfilter
	{
		nearest = 0,
		linear = 1,
		anisotropy = 2,
	};

	class DSEEDEXP sprite_atlas : public object, public wrapped
	{
	public:
		virtual size2i size() noexcept = 0;
		virtual color::pixelformat format() noexcept = 0;

	public:
		virtual rect2i atlas_element(size_t index) noexcept = 0;
		virtual size_t atlas_count() noexcept = 0;
	};

	class DSEEDEXP sprite_rendertarget : public object, public wrapped
	{
	public:
		virtual size2i size() noexcept = 0;
		virtual color::pixelformat format() noexcept = 0;

	public:
		virtual error_t atlas(sprite_atlas** atlas) noexcept = 0;
	};

	class DSEEDEXP sprite_render : public vgarender
	{
	public:
		virtual error_t create_pipeline(shaderpack* pixelshader, const blendparams* blendparams, sprite_texfilter texfilter, pipeline** pipeline) noexcept = 0;
		virtual error_t create_atlas(bitmaps::bitmap* bitmap, const rect2i* atlases, size_t atlas_count, sprite_atlas** atlas) noexcept = 0;
		virtual error_t create_rendertarget(const size2i& size, color::pixelformat format, sprite_rendertarget** target) noexcept = 0;
		virtual error_t create_constant(size_t size, vgabuffer** constbuf, const void* dat = nullptr) noexcept = 0;

	public:
		virtual error_t set_pipeline(pipeline* pipeline) noexcept = 0;
		virtual error_t set_rendertarget(sprite_rendertarget** rendertargets, size_t size) noexcept = 0;
		virtual error_t set_atlas(sprite_atlas** atlas, size_t size) noexcept = 0;
		virtual error_t set_constant(vgabuffer** constbuf, size_t size) noexcept = 0;

	public:
		virtual error_t clear_rendertarget(sprite_rendertarget* rendertarget, const dseed::f32x4_t& color) noexcept = 0;
		virtual error_t update_constant(vgabuffer* constbuf, const void* buf, size_t offset, size_t length) noexcept = 0;
		virtual error_t update_atlas(sprite_atlas* atlas, dseed::bitmaps::bitmap* data) noexcept = 0;

	public:
		virtual error_t begin(rendermethod method, const f32x4x4_t& transform) noexcept = 0;
		virtual error_t end() noexcept = 0;

	public:
		virtual error_t draw(size_t atlas_index, const f32x4x4_t& transform, const f32x4_t& color) noexcept = 0;
	};
}

#if PLATFORM_MICROSOFT
#	if defined(USE_D3D11_NATIVE_OBJECT)
#		include <d3d11.h>
#		include <dxgi.h>
#		include <wrl.h>
#	endif
#endif
namespace dseed::graphics
{
#if PLATFORM_MICROSOFT && defined(USE_D3D11_NATIVE_OBJECT)
	struct d3d11_vgadevice_nativeobject
	{
		Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice;
		Microsoft::WRL::ComPtr<IDXGISwapChain> dxgiSwapChain;
	};

	struct d3d11_sprite_vgabuffer_nativeobject
	{
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	};

	struct d3d11_sprite_atlas_nativeobject
	{
		Microsoft::WRL::ComPtr<ID3D11Resource> texture;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView;
	};

	struct d3d11_sprite_rendertarget_nativeobject
	{
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;
	};

	struct d3d11_sprite_pipeline_nativeobject
	{
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
		Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;
		Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;
	};
#endif
#if PLATFORM_MICROSOFT && defined(USE_D3D12_NATIVE_OBJECT)

#endif
#if defined(USE_OPENGL_NATIVE_OBJECT)

#endif
#if defined(USE_VULKAN_NATIVE_OBJECT)

#endif
#if defined(USE_METAL_NATIVE_OBJECT)

#endif
}

namespace dseed::graphics
{
	DSEEDEXP error_t create_dxgi_vgaadapter_enumerator(vgaadapter_enumerator** enumerator) noexcept;
	DSEEDEXP error_t create_d3d11_vgadevice(platform::application* app, vgaadapter* adapter, vgadevice** device) noexcept;

	DSEEDEXP error_t create_d3d12_vgadevice(platform::application* app, vgaadapter* adapter, vgadevice** device) noexcept;
	
	DSEEDEXP error_t create_opengl_vgadevice(platform::application* app, vgaadapter* adapter, vgadevice** device) noexcept;
	
	DSEEDEXP error_t create_vulkan_vgaadapter_enumerator(vgaadapter_enumerator** enumerator) noexcept;
	DSEEDEXP error_t create_vulkan_vgadevice(platform::application* app, vgaadapter* adapter, vgadevice** device) noexcept;
	
	DSEEDEXP error_t create_metal_vgadevice(platform::application* app, vgaadapter* adapter, vgadevice** device) noexcept;
}

#endif