/*
 * Copyright (C) 2014 NVIDIA Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __SOC_TEGRA_COMMON_H__
#define __SOC_TEGRA_COMMON_H__

#include <linux/platform_device.h>

struct board_info {
	u16 board_id;
	u16 sku;
	u8  fab;
	u8  major_revision;
	u8  minor_revision;
};

extern phys_addr_t tegra_bootloader_fb_start;
extern phys_addr_t tegra_bootloader_fb_size;
extern phys_addr_t tegra_bootloader_fb2_start;
extern phys_addr_t tegra_bootloader_fb2_size;
extern phys_addr_t tegra_fb_start;
extern phys_addr_t tegra_fb_size;
extern phys_addr_t tegra_fb2_start;
extern phys_addr_t tegra_fb2_size;

bool soc_is_tegra(void);
int tegra_get_usb_port_owner_info(void);
void tegra_get_display_board_info(struct board_info *bi);
int tegra_get_board_panel_id(void);
void __tegra_move_framebuffer(struct platform_device *pdev,
				phys_addr_t to, phys_addr_t from, size_t size);
void __tegra_clear_framebuffer(struct platform_device *pdev,
				unsigned long to, unsigned long size);


#endif /* __SOC_TEGRA_COMMON_H__ */
