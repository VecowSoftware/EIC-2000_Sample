#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <linux/watchdog.h>
#include <sys/ioctl.h>
#include <unistd.h>


int main(int argc, char	*argv[])
{

     int fd,timeout;
     
     //開啟watchdog file 
	 fd = open("/dev/watchdog",O_RDWR);


     //設定多久沒被喚醒重開機 單位:秒 
	 timeout = 3;
	 
	 
	 //啟動watchdog
     ioctl(fd , WDIOC_SETTIMEOUT ,&timeout); 

     
     printf("Watchdog start. System will reboot after 3 seconds\n");

/*
	while(1)
    {
    //喚醒    
    ioctl(fd , WDIOC_KEEPALIVE,0); 

	sleep(1);
	}*/
    close (fd);


	return 0;
}
