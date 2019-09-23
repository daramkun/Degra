#ifndef __DSEED_GRAPHICS_D3D11_H__
#define __DSEED_GRAPHICS_D3D11_H__

namespace dseed
{
	DSEEDEXP error_t create_d3d11_vga_device (vga_adapter* adapter, vga_device** device);
}

#endif