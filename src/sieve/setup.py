from setuptools import setup, Extension
import os
import platform

# Set ARCHFLAGS for correct architecture
arch_flags = os.getenv('ARCHFLAGS', None)
if not arch_flags:
    system_arch = platform.machine()
    if system_arch == 'x86_64':
        os.environ['ARCHFLAGS'] = "-arch x86_64"
    elif system_arch == 'arm64':
        os.environ['ARCHFLAGS'] = "-arch arm64"
    else:
        raise ValueError(f"Unsupported architecture: {system_arch}")

# Set ARCHFLAGS for correct architecture
arch_flags = os.getenv('ARCHFLAGS', None)
if not arch_flags:
    system_arch = platform.machine()
    if system_arch == 'x86_64':
        os.environ['ARCHFLAGS'] = "-arch x86_64"
    elif system_arch == 'arm64':
        os.environ['ARCHFLAGS'] = "-arch arm64"
    else:
        raise ValueError(f"Unsupported architecture: {system_arch}")

header_dirs = ['sieve.h','sieve_exceptions.h']

# Define the C extension module
c_extension = Extension(
    name="sieve",  # The name Python will use to import your module
    sources=["python_wrapper.c", "sieve.c", "sieve_exceptions.c"],  # Your C wrapper source file
    include_dirs=header_dirs,  # Include directories for header files
    # include_dirs=[".", "/path/to/your/corelib/headers"], # Include directories for your C code
    # library_dirs=["/path/to/your/corelib/lib"],     # Library directories if linking against a library
    # libraries=["corelib"],                            # Libraries to link against
    extra_compile_args=["-std=c23",'-Wall'],                 # Optional: C compiler flags
    # extra_link_args=[],                              # Optional: Linker flags
)

setup(
    name="sieve",  # The name of your Python package
    version="0.1.0",
    description="A Python package with a C extension",
    ext_modules=[c_extension],
    # Other setup parameters like author, license, etc.
)