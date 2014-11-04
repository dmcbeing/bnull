#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/genhd.h>
#include <linux/bio.h>

#include "bnull_devops.h"


int bnull_ioctl(struct block_device * bdev,fmode_t filemod,unsigned int ioctl_num,unsigned long ioctl_param)
{
	switch(ioctl_num)
	{
		default:
			printk("IOCTL(%u,%ulx)\n",ioctl_num,ioctl_param);
			;;
	}
	return 0;
}

void bnull_make_request(struct request_queue *q, struct bio *bio)
{
	bio_endio(bio,0);
}