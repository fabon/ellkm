#!/bin/bash

##
## Copyright 2013 Fabon Dzogang
##
## This file is part of ellkm.

## ellkm is free software: you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## any later version.

## ellkm is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.

## You should have received a copy of the GNU General Public License
## along with ellkm.  If not, see <http://www.gnu.org/licenses/>.
##

echo "Toy dataset in a streaming scenario with the spherical Kmeans"
read -p "Press [Enter] to run example..."
./ellkm examples/toy_data 0 2

echo "Toy dataset in a streaming scenario with the Ellipsoidal Kmeans"
read -p "Press [Enter] to run example..."
./ellkm examples/toy_data 0.3 2

echo "Small sample of a real dataset in a batch scenario with the Ellipsoidal Kmeans"
read -p "Press [Enter]  to run example..."
./ellkm examples/real_small 0.14 2
