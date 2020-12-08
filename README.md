# coe322-tsp

This repository contains our (Braden Pecora and Harrison Jin) final project for COE 322 Fall 2020. This project is the Amazon Delivery Truck project. See the PDF file for full writeup.

## Prerequisites

To generate the maps, we used [MatPlotLib](https://matplotlib.org).

```
python3 -m pip install -U pip
python3 -m pip install -U matplotlib
```

## Usage

Change the first line of `run_demo.sh` to whatever your C++ compiler is.

```
sh run_demo.sh
```

This script compiles and runs the program, which includes a demonstration of various scenarios as described in the write-up. It also saves the routes to the `routes` folder, then generates and saves maps of each route to the `figures` folder. To continue to the next scenario, simply close all of the currently showing maps. To use custom test data, replace the corresponding files in the `data`.