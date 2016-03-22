# What?

fgstd is sort of a replacement for parts the STL.

It it neither designed nor inteded to be compatible with the STL, and there are trade-offs towards a simplified API and higher performance.

It is currently a work-in-progress and not ready for use.

# Provided functionality

- Unified primitive types (even without stdint.h)
- Type traits
- Containers
	- vector
	- hasmap
	- multivector (SOA-friendly, WIP)

### NYI

- Algorithms
	- sort
	- heap
	- ...
- Utility
	- bytebuf
	- bitbuf
	- streams?
- More containers
	- treemap
	- splaymap?
	- flatmap
	- linkedlist? (really?!)
	- flatlist
- Automatic expression vectorizer via expression templates (WIP)

# Design goals

- 100% customizable memory allocation
	* Allocators are not part of the type signature
	* Instead: virtual methods (you're not going to spam memory allocation, are you?)
	* Alignment-aware
- No exceptions!
- C++03 compatible, but can use C++11 if svailable
- No memory allocation at all for empty containers
- Data structures are small (as in sizeof()) and simple (keeping the code size down)
- Separated declaration and implementation headers for faster compile times
- Basic operations pretty much STL-compatible (so you'll feel right at home)
- Depends only on some parts of the C library:
	- memcpy(), memmove(), memcmp()


# Excluded

Aka functionality that almost nobody uses or needs

- Locales
- ? Printing functionality (cout, cin, etc)
- wchat_t, wstring, etc

