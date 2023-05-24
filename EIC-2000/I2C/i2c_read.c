#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

#define I2C_ADAPTER "/dev/i2c-4"
#define I2C_ADDR 0x20 
#define I2C_REG_ADDR 0x20


int fd;




int i2c_read(int fd, char addr, char reg, char *val)
{
     if(ioctl(fd, I2C_SLAVE, addr)<0) {
            printf("fail to set i2c device slave address!\n");
            close(fd);
            return -1;
        }

     if(write(fd, &reg, 1)){
         if(read(fd, val, 1)){
                 return 0;
        }
    }
	return -1;
}




int main(int argc,char *argv[])
{       
	char val;
	fd = open(I2C_ADAPTER, O_RDWR);
        if(fd < 0)
        {
            perror("Can't open I2C device.\n");
            return -1;
        }
	i2c_read(fd,I2C_ADDR,I2C_REG_ADDR,&val);
	printf("i2c device:%s, i2c addr:%d, i2c reg addr:%d, value=%d.\n",I2C_ADAPTER,I2C_ADDR,I2C_REG_ADDR,val);
	return 0;
}
