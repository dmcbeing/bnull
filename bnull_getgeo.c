#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/genhd.h>
#include <linux/hdreg.h>

#include "bnull_getgeo.h"

int bnull_getgeo(struct block_device * bdev, struct hd_geometry * geom)
{
	geom->heads = 1;
	geom->cylinders = 1;
	geom->sectors = get_capacity(bdev->bd_disk);
	geom->start = 0;
	return 0;
}
