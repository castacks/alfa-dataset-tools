# Fault Detection

The AirLab - Carnegie Mellon University

## Getting Started

Please make sure that you have successfully installed the following dependencies before trying to run this package.
### Dependencies:
---------------------

* [Boost](https://www.boost.org/) - Boost C++ Libraries (V >= 1.43)
* [CMake](https://cmake.org/) - CMake open-source, cross-platform (V >= 2.6)

#### Installing dependencies:

To install Boost, run the following command:

```
sudo apt-get update
sudo apt-get install libboost-all-dev
```

To install Cmake, run the following command:

```
sudo apt-get update
sudo apt-get install build-essential
sudo apt-get install cmake
```

### Building this package
---------------------
Run the following to build the library:

```
    mkdir build
    cd build
    cmake ..
    make
```

To run the main code:

```
    ./sample -d DIRECTORY-TO-CSV-FILE(e.g. csv/sample.csv)
```

## Author

* **Mohammadreza Mousaei** - [mmousaei](https://github.com/mmousaei)

## License

This project is licensed under the Carnegie Mellon University License

## Acknowledgments

NASA Ack goes here
