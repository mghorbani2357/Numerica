from setuptools import setup, Extension
import os

# Define the C extension module
c_extension = Extension(
    name="numericalib",  # The name Python will use to import your module
    sources=["python_wrapper.c"],  # Your C wrapper source file
    # include_dirs=[".", "/path/to/your/corelib/headers"], # Include directories for your C code
    # library_dirs=["/path/to/your/corelib/lib"],     # Library directories if linking against a library
    # libraries=["corelib"],                            # Libraries to link against
    extra_compile_args=["-std=c23"],                 # Optional: C compiler flags
    extra_link_args=[],                              # Optional: Linker flags
)

setup(
    name="numericalib",  # The name of your Python package
    version="0.1.0",
    description="A Python package with a C extension",
    ext_modules=[c_extension],
    # Other setup parameters like author, license, etc.
)