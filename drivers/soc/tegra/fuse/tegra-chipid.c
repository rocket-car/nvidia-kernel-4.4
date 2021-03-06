/*
 * Copyright (c) 2016, NVIDIA CORPORATION.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/tegra-soc.h>

#define TEGRAID_CHIPID_MASK 0xFF00
#define TEGRAID_CHIPID_SHIFT 8
#define TEGRAID_MAJOR_MASK 0xF0
#define TEGRAID_MAJOR_SHIFT 4
#define TEGRAID_MINOR_MASK 0xF0000
#define TEGRAID_MINOR_SHIFT 16
#define TEGRAID_NETLIST_MASK 0xFF
#define TEGRAID_PATCH_MASK 0xFF00
#define TEGRAID_PATCH_SHIFT 8

struct tegra_id {
	enum tegra_chipid chipid;
	enum tegra_revision revision;
	unsigned int major;
	unsigned int minor;
	unsigned int netlist;
	unsigned int patch;
	char *priv;
};

static const char *tegra_platform_name[TEGRA_PLATFORM_MAX] = {
	[TEGRA_PLATFORM_SILICON] = "silicon",
	[TEGRA_PLATFORM_QT]      = "quickturn",
	[TEGRA_PLATFORM_LINSIM]  = "linsim",
	[TEGRA_PLATFORM_FPGA]    = "fpga",
	[TEGRA_PLATFORM_UNIT_FPGA] = "unit fpga",
	[TEGRA_PLATFORM_VDK] = "vdk",
};

static struct tegra_id tegra_id;
static const char *tegra_platform_ptr;
static const char *tegra_cpu_ptr;

static int get_platform(char *val, const struct kernel_param *kp)
{
	enum tegra_platform platform;

	platform = tegra_get_platform();
	tegra_platform_ptr = tegra_platform_name[platform];
	return param_get_charp(val, kp);
}
static struct kernel_param_ops tegra_platform_ops = {
	.get = get_platform,
};
module_param_cb(tegra_platform, &tegra_platform_ops, &tegra_platform_ptr, 0444);

static int get_cpu_type(char *val, const struct kernel_param *kp)
{
	enum tegra_platform platform;

	platform = tegra_get_platform();
	tegra_cpu_ptr = tegra_platform_name[platform];
	return param_get_charp(val, kp);
}
static struct kernel_param_ops tegra_cpu_ops = {
	.get = get_cpu_type,
};
module_param_cb(tegra_cpu, &tegra_cpu_ops, &tegra_cpu_ptr, 0444);

static int get_chip_id(char *val, const struct kernel_param *kp)
{
	return param_get_uint(val, kp);
}

static int get_revision(char *val, const struct kernel_param *kp)
{
	return param_get_uint(val, kp);
}

static struct kernel_param_ops tegra_chip_id_ops = {
	.get = get_chip_id,
};

static struct kernel_param_ops tegra_revision_ops = {
	.get = get_revision,
};

module_param_cb(tegra_chip_id, &tegra_chip_id_ops, &tegra_id.chipid, 0444);
module_param_cb(tegra_chip_rev, &tegra_revision_ops, &tegra_id.revision, 0444);

void tegra_set_tegraid_from_hw(void)
{
	u32 cid;
	u32 emu_id;

	cid = tegra_read_chipid();
	emu_id = tegra_read_emu_revid();

	tegra_id.chipid  = (cid & TEGRAID_CHIPID_MASK) >> TEGRAID_CHIPID_SHIFT;
	tegra_id.major = (cid & TEGRAID_MAJOR_MASK) >> TEGRAID_MAJOR_SHIFT;
	tegra_id.minor   = (cid & TEGRAID_MINOR_MASK) >> TEGRAID_MINOR_SHIFT;
	tegra_id.netlist = emu_id & TEGRAID_NETLIST_MASK;
	tegra_id.patch   = (emu_id & TEGRAID_PATCH_MASK) >> TEGRAID_PATCH_SHIFT;
	tegra_id.revision = tegra_sku_info.revision;
}

enum tegra_chipid tegra_get_chipid(void)
{
	if (tegra_id.chipid == TEGRA_CHIPID_UNKNOWN)
		tegra_set_tegraid_from_hw();

	return tegra_id.chipid;
}
