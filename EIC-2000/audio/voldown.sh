sudo echo 130 > /sys/class/gpio/export
sudo echo out > /sys/class/gpio/gpio130/direction
sudo echo 1 > /sys/class/gpio/gpio130/value
sleep 0.2
sudo echo 0 > /sys/class/gpio/gpio130/value
sleep 0.2
sudo echo 1 > /sys/class/gpio/gpio130/value
