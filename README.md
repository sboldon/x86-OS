
# x86 OS

A toy x86 operating system written with the goal of understanding more about the interface between hardware and software.

## Building

A cross-compiler targeting i686-elf is required. As is, the makefile expects `i686-elf-gcc` to be in `PATH`. Invoking `make` creates and installs all build artifacts into directory `./_build`.

Bochs or QEMU can be used for emulation:
```
qemu-system-i386 ./_build/kernel.img
```

---

##### Development Resources:

* Nick Blundell's book "Writing a Simple Operating System From Scratch"
* [OSDev Wiki](https://wiki.osdev.org/)
* [FreeVGA Project](http://www.osdever.net/FreeVGA/home.htm)
