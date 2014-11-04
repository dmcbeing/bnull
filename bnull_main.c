/*
	bnull: A block based /dev/null device.
*/

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/bio.h>
#include <linux/blkdev.h>
#include <linux/genhd.h>

#include "bnull_devops.h"
#include "bnull_getgeo.h"


MODULE_LICENSE( "GPL" );
MODULE_AUTHOR( "Mavridis Stelios" );
MODULE_SUPPORTED_DEVICE( "bnull" );
MODULE_DESCRIPTION( "Block based /dev/null device." );

struct block_device_operations bnull_dops =
{
//	.open = 0,
//	.release = 0,
//	.rw_page = 0,
	.ioctl = bnull_ioctl,
	.compat_ioctl = bnull_ioctl,
//	int (*direct_access) (struct block_device *, sector_t,void **, unsigned long *);
//	unsigned int (*check_events) (struct gendisk *disk,unsigned int clearing);
//	int (*media_changed) (struct gendisk *);
//	void (*unlock_native_capacity) (struct gendisk *);
//	int (*revalidate_disk) (struct gendisk *);
	.getgeo = bnull_getgeo,
//	int (*getgeo)(struct block_device *, struct hd_geometry *);
	/* this callback is with swap_lock and sometimes page table lock held */
//	void (*swap_slot_free_notify) (struct block_device *, unsigned long);
	.owner = THIS_MODULE
};

/* Module Global Variables */
int major = 0;
struct gendisk * bnull_gendisk = 0;

/* Module Parameters */
int bnull_minors = 5; 		/* Number of partitions our bnull device can have including the actual /dev/bnull device.The default value 5 allows /dev/bnull to have 4 partitions */
uint64_t bnull_size = 1024l*1024l*1024l*1024l;	/* As we have a block device we must set the number of 4k blocks.The default value set the device to 4Tb*/

static int bnull_init( void );
static void bnull_exit( void );

static int bnull_init()
{
	printk( KERN_INFO "bnull:Created /dev/bnull\n" );
	major = register_blkdev( major, "bnull" );

	if(major <= 0)
		return -EBUSY;

	bnull_gendisk = alloc_disk(bnull_minors);	/* Alloc disk creates the data structures representing the new device */
	bnull_gendisk->major = major;				/* Here we give the device our registered major,this allows Linux to understand which driver implements the device's functionality */
	bnull_gendisk->first_minor = 0;				/* As we have only one device (bnulla) we start from 0.If we were to support multiple devices we would have to advance this by bnull_minors for every new device */
	bnull_gendisk->fops = &bnull_dops;
	snprintf (bnull_gendisk->disk_name, 7, "bnulla");					/* Here we set the devices name */
	set_capacity(bnull_gendisk, bnull_size/8);							/* Here we set the devices size */
	bnull_gendisk->queue = blk_alloc_queue(GFP_KERNEL);					/* We allocate the device queue */
	blk_queue_make_request(bnull_gendisk->queue,bnull_make_request);	/* Here we set our make_request function */
	add_disk(bnull_gendisk);											/* At this point all necessary functions/structs have been initialized,and we export the device to the system. */
	return 0;															/* By returning 0 we ensure that everything went fine,if a non zero value was returned the module would not be loaded */
}

static void bnull_exit()
{
	del_gendisk( bnull_gendisk );										/* Reverse of add_disk.*/
	unregister_blkdev( major, "bnull" );								/* Unregister block device driver */
	printk( KERN_INFO "bnull:Destroyed /dev/bnull\n" );
}

module_init( bnull_init );
module_exit( bnull_exit );
