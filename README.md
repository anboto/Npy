# Npy
[.npy](https://numpy.org/devdocs/reference/generated/numpy.lib.format.html) is the standard NumPy file format for saving arrays to disk with the full information about them. [.npz](https://numpy.org/devdocs/reference/generated/numpy.lib.format.html) is the standard format for persisting multiple NumPy arrays on disk. A .npz file is a zip file containing multiple .npy files, one for each array.

[libnpy](https://github.com/llohse/libnpy) is a simple C++ library for reading and writing of NumPy's .npy files.

The Npy package is a basic C++ wrapper for Npy/Npz file format. For ease of use, a simple example is included.
