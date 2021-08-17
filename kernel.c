#if defined(__linux__)
	#error "Kernel must be built with a cross-compiler"
#elif !defined(__i386__)
	#error "Kernel must be built with an x86-elf compiler"
#endif

// Example monolithic kernel
int main() {
  char* video_mem = (char*)0xb8000;
  *video_mem = 'K';
}

