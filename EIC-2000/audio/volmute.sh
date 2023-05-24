sudo echo 127 > /sys/class/gpio/export
sudo echo out > /sys/class/gpio/gpio127/direction
sudo echo 1 > /sys/class/gpio/gpio127/value
sleep 0.2
sudo echo 0 > /sys/class/gpio/gpio127/value
sleep 0.2
sudo echo 1 > /sys/class/gpio/gpio127/value

