#!/bin/sh

echo "Installing dependencies for RaumScanner!"
sudo apt-get -y update
sudo apt-get -y install python-smbus libi2c-dev libglm-dev make gcc g++ i2c-tools

echo "Now adding pi to i2c group"
sudo usermod -aG i2c pi


sudo echo "i2c-dev" >> /etc/modules
sudo modprobe i2c-bcm2708
sudo modprobe i2c_dev
sudo echo "dtparam=i2c_arm=on" >> /boot/config.txt
