sudo echo 128 > /sys/class/gpio/export
sudo echo out > /sys/class/gpio/gpio128/direction
sudo echo 1 > /sys/class/gpio/gpio128/value
sleep 0.2
sudo echo 0 > /sys/class/gpio/gpio128/value
sleep 0.2
sudo echo 1 > /sys/class/gpio/gpio128/value
