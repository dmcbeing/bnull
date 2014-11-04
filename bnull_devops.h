#ifndef BNULL_IOCTL_HEADER_LOCK
	#define BNULL_IOCTL_HEADER_LOCK

	int bnull_ioctl(struct block_device * bdev,fmode_t filemod,unsigned int ioctl_num,unsigned long ioctl_param);

	void bnull_make_request(struct request_queue *q, struct bio *bio);
#endif