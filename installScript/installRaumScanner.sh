#!/bin/sh

echo "Benutzung auf eigene Gefahr!"
echo "Funktionsfähigkeit wird nicht garantiert."
echo "Insbesondere muss noch die Datei /etc/modprobe.d/raspi-blacklist.conf überprüft werden."
echo "Dort dürfen die Zeilen 'i2c-bcm2708' und 'spi-bcm2708' nicht vorhanden (bzw. auskommentiert) sein"
echo "Zum Ausführen des Skriptes die Zeile mit dem exit entfernen."
echo "Der Rest müsste (sollte) funktionieren. Ist aber ungetestet."
echo "Viel Spaß beim Projekt!"
echo "Viele Grüße vom Wintersemester 2015/16"
exit 201516;

echo "Installing dependencies for RaumScanner!"
sudo apt-get -y update
sudo apt-get -y install python-smbus libi2c-dev libglm-dev make gcc g++ i2c-tools

echo "Now adding pi to i2c group"
sudo usermod -aG i2c pi


sudo echo "i2c-dev" >> /etc/modules
sudo modprobe i2c-bcm2708
sudo modprobe i2c_dev
sudo echo "dtparam=i2c_arm=on" >> /boot/config.txt
