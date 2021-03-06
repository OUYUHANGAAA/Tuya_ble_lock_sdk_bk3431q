# Mbed Crypto library

The Mbed cryptography library is a reference implementation of the cryptography interface of the Arm Platform Security (PSA) architecture. This is a preview release of Mbed Crypto, provided for evaluation purposes only.

Mbed Crypto is distributed under the Apache License, version 2.0. See the [LICENSE](LICENSE) file for the full text of the license.

## PSA cryptography API

Arm's Platform Security Architecture (PSA) is a holistic set of threat models, security analyses, hardware and firmware architecture specifications, and an open source firmware reference implementation. PSA provides a recipe, based on industry best practice, that allows security to be consistently designed in, at both a hardware and firmware level.

The PSA Cryptography API provides access to a set of cryptographic primitives. It has a dual purpose: it can be used in a PSA-compliant platform to build services such as secure boot, secure storage and secure communication; and it can also be used independently of other PSA components on any platform.

The design goals of the PSA Cryptography API include:

* The API distinguishes caller memory from internal memory, which allows the library to be implemented in an isolated space for additional security. Library calls can be implemented as direct function calls if isolation is not desired, and as remote procedure calls if isolation is desired.
* The structure of internal data is hidden to the application, which allows substituting alternative implementations at build time or run time, for example in order to take advantage of hardware accelerators.
* All access to keys is done via handles, which allows support for external cryptoprocessors that is transparent to applications.
* The interface to algorithms is generic, favoring algorithm agility.
* The interface is designed to be easy to use, and hard to accidentally misuse.

## Mbed Crypto implementation

Mbed Crypto is a reference implementation of the PSA Cryptography API. It is written in portable C.

## Documentation

The Mbed Crypto library is a reference implementation of the PSA Cryptography API. Therefore, the library's API documentation is the PSA Cryptography API specification. The PSA Cryptography API specification consists of the following documents:

* The [PSA Cryptography API overview](docs/PSA_Crypto_API_Overview.pdf).
* The [PSA Cryptography API detailed function reference](docs/PSA_Crypto_API_Reference.pdf), which you can also browse in [HTML format](docs/html/modules.html).

## Compiling

You need the following tools to build the library with the provided makefiles:

* GNU Make.
* A C toolchain (compiler, linker, archiver).
* Python 2 or Python 3 (either will work) to generate the test code.
* Perl to run the tests.

If you have a C compiler such as GCC or Clang, just run `make` in the toplevel directory to build the library, a set of unit tests and some sample programs.

To select a different compiler, set the `CC` variable to name or path of the compiler and linker (default: `cc`) and set `AR` to a compatible archiver (default: `ar`). For example:
```
make CC=arm-linux-gnueabi-gcc AR=arm-linux-gnueabi-ar
```
The provided makefiles pass options to the compiler that assume a GCC-like command line syntax. To use a different compiler, you may need to pass different values for `CFLAGS`, `WARNINGS_CFLAGS` and `LDFLAGS`.

To run the unit tests on the host machine, run `make test` from the toplevel directory. If you are cross-compiling, copy the test executable from the `tests` directory to the target machine.

## Example programs

The `programs/` subdirectory contains some sample programs that use the library. Please note that the goal of these sample programs is to demonstrate specific features of the library and the code may need to be adapted to build a real-world application.

## Upcoming features

Future releases of this library will include:

* A driver programming interface, to use hardware accelerators instead of the default software implementation for chosen algorithms.
* Support for external keys, stored and manipulated exclusively in a separate cryptoprocessor.
* A configuration mechanism to compile only the algorithms you need for your application.
* A wider set of cryptographic algorithms.

## Feedback welcome

Arm welcomes feedback on the design of the API. If you think something could be improved, please open an issue on our Github repository. Alternatively, if you prefer to provide your feedback privately, please email us at [`mbed-crypto@arm.com`](mailto:mbed-crypto@arm.com). All feedback received in email will be treated confidentially.
