# LidarWS1718

## Meilensteine

* [x] Merlin-Steuerung zum Laufen bekommen

* [ ] Gyro auslesen können

* [ ] Verbindung zwischen Motorsteuerung und Gyro herstellen

* [ ] LIDAR zum Laufen bekommen

* [ ] Aus den Teilelementen ein sinnvolles Programm formen, das folgendes kann:
  * [ ] Merlin und Gyro kalibrieren/nullen
  * [ ] Den Merlin so ansteuern, dass er die Hemisphäre scannt
  * [ ] Dabei in regelmäßigen Intervallen Messungen mit Gyroskop und Lidar machen und sammeln
  * [ ] Abschließend die Punktwolke in einem 3D-Format (OBJ, PLY o.ä.) rausschreiben (Format siehe Aufgabenstellung)

* [ ] LIDAR Daten in einem sinnvollen Format Speichern

* [ ] LIDAR Punktwolken in Meshes umwandeln


## Links
- [Merlin Kopf + Gyro SoSe 2016](http://iwi-i-lfm.hs-karlsruhe.de/foswiki/bin/view/IWI/AutonomeSysteme/SoSe2016Lidar)
- [LIDAR WiSe 2015](http://iwi-i-lfm.hs-karlsruhe.de/foswiki/bin/view/IWI/AutonomeSysteme/WiSe2015RaumScanner)


## Remotes
Origin Sourceforge: https://iwistudent@git.code.sf.net/p/autosyslab/laserscanner
Origin Github: https://github.com/dojo1017/LidarWS1718.git


## Minidoku

UART zum Laufen bringen (Raspberry Pi 3):

1. sudo apt-get update && sudo apt-get upgrade -y
1. sudo rpi-update
1. sudo reboot
1. sudo nano /boot/cmdline.txt und "console=serial0,115200" entfernen, speichern
1. sudo nano /boot/config.txt, folgendes eintragen:
  dtoverlay=pi3-miniuart-bt
  enable_uart=1
1. sudo reboot