#ifndef __DSEED_GRAPHICS_H__
#define __DSEED_GRAPHICS_H__

namespace dseed
{
	struct DSEEDEXP displaymode
	{
		size2i resolution;
		pixelformat_t format;
		fraction refresh_rate;
	};

	class DSEEDEXP display : public object, public wrapped
	{
	public:
		virtual error_t name (char* name, size_t maxNameCount) = 0;

	public:
		virtual error_t displaymode (int index, displaymode* mode) = 0;
		virtual size_t displaymode_count () = 0;

	public:
		virtual error_t refresh () = 0;
	};

	class DSEEDEXP vga_adapter : public object, public wrapped
	{
	public:
		virtual error_t name (char* name, size_t maxNameCount) = 0;

	public:
		virtual error_t display (int index, display** display) = 0;
		virtual size_t display_count () = 0;

	public:
		virtual error_t refresh () = 0;
	};

	class DSEEDEXP vga_adapter_enumerator : public object
	{
	public:
		virtual error_t vga_adapter (int index, vga_adapter** adapter) = 0;
		virtual size_t vga_adapter_count () = 0;

	public:
		virtual error_t refresh () = 0;
	};

	class texture;
	class texture1d;
	class texture2d;
	class texture3d;
	class texturecube;

	class rendertarget;
	class rendertarget2d;

	class buffer;
	class vertexbuffer;
	class indexbuffer;
	class constantbuffer;
	class unorderedbuffer;

	class shader;
	class vertexshader;
	class pixelshader;
	class geometryshader;
	class hullshader;
	class domainshader;
	class computeshader;
	class program;

	class pipeline;

	enum clearflag_t
	{
		clearflag_none = 0,

		clearflag_color = 1,
		clearflag_depth = 2,
		clearflag_stencil = 4,

		clearflag_all = clearflag_color | clearflag_depth | clearflag_stencil
	};

	struct DSEEDEXP viewport
	{
		rectanglef viewport;
		float znear, zfar;
	};

	class DSEEDEXP vga_commandbuffer : public object, public wrapped
	{
	public:
		virtual error_t set_rendertarget (clearflag_t cf, rendertarget** rendertargets, size_t rendertargetCount, rendertarget* depthstencil) = 0;
		virtual error_t clear_rendertargets (const rgba& color, float depth, float stencil) = 0;

	public:
		virtual error_t set_viewport (viewport* viewports, size_t viewportCount) = 0;

	public:
		virtual error_t draw (int vertexCount, int startIndex) = 0;
		virtual error_t drawIndexed (int indexCount, int startIndex) = 0;

		virtual error_t drawInstanced (int vertexCountPerInstance, int instanceCount, int startIndex) = 0;
		virtual error_t drawIndexedInstanced (int indexCountPerInstance, int instanceCount, int startIndex) = 0;
	};

	class DSEEDEXP vga_device : public object, public wrapped
	{
	public:
		virtual error_t adapter (vga_adapter** adapter) = 0;

	public:
		virtual bool is_support_format (pixelformat_t pf) = 0;
		virtual bool is_support_parallel_render () = 0;

	public:
		virtual error_t create_texture1d (int32_t size, pixelformat_t pf, texture1d** texture) = 0;
		virtual error_t create_texture2d (const size2i& size, pixelformat_t pf, texture2d** texture) = 0;
		virtual error_t create_texture3d (const size3i& size, pixelformat_t pf, texture3d** texture) = 0;
		virtual error_t create_texturecube (const size2i& size, pixelformat_t pf, texturecube** texture) = 0;

	public:
		virtual error_t create_rendertarget2d (texture2d* texture, rendertarget2d** rendertarget) = 0;

	public:
		virtual error_t create_vertexbuffer (size_t stride, size_t length, vertexbuffer** buffer) = 0;
		virtual error_t create_indexbuffer (size_t length, int bpp, vertexbuffer** buffer) = 0;
		virtual error_t create_constantbuffer (size_t size, vertexbuffer** buffer) = 0;

	public:
		virtual error_t create_vertexshader (blob* shaderData, vertexshader** shader) = 0;
		virtual error_t create_pixelshader (blob* shaderData, pixelshader** shader) = 0;
		virtual error_t create_geometryshader (blob* shaderData, geometryshader** shader) = 0;
		virtual error_t create_hullshader (blob* shaderData, hullshader** shader) = 0;
		virtual error_t create_domainshader (blob* shaderData, domainshader** shader) = 0;
		virtual error_t create_computeshader (blob* shaderData, computeshader** shader) = 0;

	public:
		virtual error_t create_program (vertexshader* vs, pixelshader* ps, geometryshader* gs, hullshader* hs, domainshader* ds, program** program) = 0;
		virtual error_t create_program (computeshader* cs, program** program) = 0;

	public:
		virtual error_t create_context (vga_commandbuffer** contex) = 0;
	};

	class DSEEDEXP vga_swapchain : public object, public wrapped
	{
	public:
		virtual error_t backbuffer (texture2d** buf) = 0;

	public:
		virtual error_t present () = 0;
	};
}

#include "graphics.dxgi.h"
#include "graphics.d3d11.h"

#endif