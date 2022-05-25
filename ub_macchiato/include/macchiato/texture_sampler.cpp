#include "texture_sampler.hpp"

mac::texture_sampler::state* mac::texture_sampler::create(
	vk::device& logical_device,
	vk::physical_device& physical_device,
	vk::filter magnification,
	vk::filter minification,
	vk::sampler_address_mode mode_u,
	vk::sampler_address_mode mode_v,
	vk::sampler_address_mode mode_w,
	float anisotropy
)
{
	auto texture_sampler_state = new mac::texture_sampler::state {

	};

	vk::physical_device_properties physical_device_properties = {};
	vk::sampler_create_info sampler_create_info				  = {};

	vk::get_physical_device_properties(physical_device, &physical_device_properties);

	sampler_create_info.sType		 = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	sampler_create_info.magFilter	 = magnification;
	sampler_create_info.minFilter	 = minification;
	sampler_create_info.addressModeU = mode_u;
	sampler_create_info.addressModeV = mode_v;
	sampler_create_info.addressModeW = mode_w;

	sampler_create_info.anisotropyEnable		= anisotropy > 0.0f ? VK_TRUE : VK_FALSE;
	sampler_create_info.maxAnisotropy			= anisotropy;//physical_device_properties.limits.maxSamplerAnisotropy;
	sampler_create_info.borderColor				= VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	sampler_create_info.unnormalizedCoordinates = VK_FALSE;
	sampler_create_info.compareEnable			= VK_FALSE;
	sampler_create_info.compareOp				= VK_COMPARE_OP_ALWAYS;

	sampler_create_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	sampler_create_info.mipLodBias = 0.0f;
	sampler_create_info.minLod	   = 0.0f;
	sampler_create_info.maxLod	   = 0.0f;

	if (vk::create_sampler(logical_device, &sampler_create_info, nullptr, &texture_sampler_state->texture_sampler) != VK_SUCCESS)
		latte::logger::fatal(latte::logger::level::SYSTEM, "failed to create texture sampler");

	return texture_sampler_state;
}

void mac::texture_sampler::destroy(mac::texture_sampler::state* texture_sampler_state)
{
	delete texture_sampler_state;
}
