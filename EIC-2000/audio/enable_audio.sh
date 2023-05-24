echo 129  > /sys/class/gpio/export
echo out > /sys/class/gpio/gpio129/direction
echo 1 > /sys/class/gpio/gpio129/value

