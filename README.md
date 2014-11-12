# bnull
=====

Simple /dev/null replacement implemented as a device driver

This project aims to:

- Create a simple block device driver, useful as learning material
- Starting point for more advanced device drivers.

In the following sections:
- Describe what it does.
- Tell how it can be used.
- (Briefly) Explain the code.
- Compare its performance against /dev/null and /dev/zero.

## What it does:

It exports a large volatile(non-persistent) block device (/dev/bnull) similar in functionality to /den/null.
Unlike /dev/null it is bi directional,that is you can write(as /dev/null) and read(similar to /dev/zero).
Unlike /dev/zero it does not return zeroed pages,instead it only completes all I/O requests it receives.

## How is that useful:

The primary purpose of this driver is to present the absolute minimum code necessary for a Linux block  device driver.
Additionally it can be used for:

- Performance evaluation
- Fault injection
- As a starting point for more advanced device drivers.

## (Brief) Explanation of the Code

Before explaining the code , i would recommend the Linux Device Drivers book, as my driver was influenced by their _sbull_ driver.
In most part i will link to relevant chapters of the book.

### Kernel Module Hello World

Before explaining the completed bnull module we will write a hello world module.
As is customary we will begin with the simplest possible kernel module that simply prints a message.

Unlike user applications kernel modules do not have the standard file input/output streams (stdin,stdout,stderr).
The Linux kernel provides the kernel log,this log can be viewed with the _dmesg_ program.
Similar to the userspace _pritnf_ function Linux provides us with the <a href="http://www.makelinux.net/ldd3/chp-4-sect-2.shtml">_printk_</a> function.
Just like <a href="http://www.cplusplus.com/reference/cstdio/printf/">_printk_</a> <a href="http://www.makelinux.net/ldd3/chp-4-sect-2.shtml">_printk_</a> has the format:

printk(format_string,...)

Where "..." denotes a variable number of arguments.

Most character specifiers are supported:

- u,d,x,lu,ld,lx,llu,lld,llx for integers.
- sub-specifiers such as width.

Floating point specifiers (such as "%f") are not supported, as the Linux kernel does not allow floating point operations in the first place.
The reason for this seems to be portability,performance and overall code simplicity (see <a href="http://stackoverflow.com/questions/13886338/use-of-floating-point-in-the-linux-kernel">[here]</a>).

#### The Code:

````C
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

static int bnull_init( void );
static void bnull_exit( void );

static int bnull_init()
{
  printk("Hello World!");
  return 0;
}

static void bnull_exit()
{
}

module_init( bnull_init );
module_exit( bnull_exit );
````

The function _bnull_init_ as the name suggests is the initializing function of our module(similar to a Java/C++ constructor function for a class). Since every module(and programmer) follows different naming conventions we explicitly state the name of the init function with the _module_init_ function. Similarly the _module_exit_ function allows the programmer to specify the exit function (destructor function in object oriented lingo).
