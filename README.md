Jeff Probe
==========

This is a fork of the [original Black Magic Probe](https://github.com/blacksphere/blackmagic).

The original is arguably better, faster and wider supported. However, this 
project was a way to offer an affordable version and I'll rely on community
support and pull requests.

One urguably better funncton is the ability to do DEBUG and Serial communication
over a single JTAG cable when paired with a device that uses single wire JTAG.

Normally, the serial header can be used on a target for the serial port, and
shows up as the second serial device on the system, however, we can dynamically
change the pins to use the ones on the JTAG cable with the following command:

``` bash
 $ mon convert_tdio enable
```

Compilation
---

Newer toolchains can cause issues. I usually work 4_9-2014q4-20141203 found [here.](https://launchpad.net/gcc-arm-embedded/4.9/4.9-2014-q4-major/+download/gcc-arm-none-eabi-4_9-2014q4-20141203-mac.tar.bz2)

the versionfollowing version

```bash
 $ make clean
 $ make PROBE_HOST=jeff CUSTOM_SER=1
 $ dfu-util --device ,1d50:6017 -s 0x00002000:leave -D src/blackmagic.bin 
```

CUSTOM OPTIONS
---

On mac, our device shows up with a serial number /dev/tty.cuJEFF123HDC 

This can be annoy if we want to autocnnect with a gith script. We can override
the use of a serial number by doing a custom compliation such that our device
shows up with the following: /dev/cu.usbmodemJEFF1 and /dev/cu.usbmodemJEFF3

```bash
 $ make PROBE_HOST=jeff CUSTOM_SER=1
```

More
---

More helpful information can be found on the black magic probe [readme](https://github.com/blacksphere/blackmagic/blob/master/README.md#black-magic-probe), which is relevant.

See online documentation at https://github.com/blacksphere/blackmagic/wiki

Binaries from the latest automated build can be found on the release page.
