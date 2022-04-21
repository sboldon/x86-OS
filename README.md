
# x86 OS

A toy x86 operating system being written to understand more about the interface between hardware and
software.

## Building and Running

A cross-compiler targeting i686-elf is required. As is, the makefile expects `i686-elf-gcc` to be in
`PATH`. Invoking `make` initiates an out of source build that installs all artifacts into
`./_build`.

* `make run` launches QEMU using the created raw boot image `./_build/kernel.img`.
* `make run_debug` launches QEMU with GDB attached using the symbol info from `./_build/kernel.elf`.

---

##### Development Resources:

* Nick Blundell's book "Writing a Simple Operating System From Scratch"
* [OSDev Wiki](https://wiki.osdev.org/)
* [FreeVGA Project](http://www.osdever.net/FreeVGA/home.htm)
* [JamesM's Kernel Development Tutorials](http://www.jamesmolloy.co.uk/tutorial_html/)
