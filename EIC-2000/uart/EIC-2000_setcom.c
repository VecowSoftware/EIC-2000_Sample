#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h> 



#define TIOCSRS485 0x542F

struct serial_rs485 {
        unsigned int flags;                     /* RS485 feature flags */
#define SER_RS485_ENABLED               (1 << 0)        /* If enabled */
#define SER_RS485_RTS_ON_SEND           (1 << 1)        /* Logical level for
                                                           RTS pin when
                                                           sending */
#define SER_RS485_RTS_AFTER_SEND        (1 << 2)        /* Logical level for
                                                           RTS pin after sent*/
#define SER_RS485_RX_DURING_TX          (1 << 4)
        unsigned int delay_rts_before_send;     /* Delay before send (milliseconds) */
        unsigned int delay_rts_after_send;      /* Delay after send (milliseconds) */
        unsigned int padding[5];                /* Memory is cheap, new structs
                                           are a royal PITA .. */
};



void chane_mode_232(int fd)
{

     /* Set RS485 mode */
     struct serial_rs485 rs485conf;
     rs485conf.flags &= ~SER_RS485_ENABLED;
     rs485conf.flags &= ~SER_RS485_RTS_ON_SEND;
     rs485conf.flags |= SER_RS485_RTS_AFTER_SEND;
     rs485conf.flags &= ~SER_RS485_RX_DURING_TX;
     rs485conf.delay_rts_before_send = 0;
     rs485conf.delay_rts_after_send = 0;
     ioctl(fd, TIOCSRS485, &rs485conf);
     return;

}


void chane_mode_485_422(int fd)
{

     /* Set RS485 mode */
     struct serial_rs485 rs485conf;
     rs485conf.flags |= SER_RS485_ENABLED;
     rs485conf.flags &= ~SER_RS485_RTS_ON_SEND;
     rs485conf.flags |= SER_RS485_RTS_AFTER_SEND;
     rs485conf.flags |= SER_RS485_RX_DURING_TX;
     rs485conf.delay_rts_before_send = 0;
     rs485conf.delay_rts_after_send = 0;
     ioctl(fd, TIOCSRS485, &rs485conf);
     return;

}


void set_gpio(int igpio, int ilevel)
{
        char buf[200];
	sprintf(buf,"sudo echo %d > /sys/class/gpio/export",igpio);
	system(buf);
	sprintf(buf,"sudo echo out > /sys/class/gpio/gpio%d/direction",igpio);
	system(buf);
	sprintf(buf,"sudo echo %d > /sys/class/gpio/gpio%d/value" ,ilevel ,igpio);
	system(buf);
}





void change_io_sta(int portnum,int mode)
{
        int iMode0 = 0, iMode1 = 0; 
	
	if(portnum == 1)
	{
        	iMode0 = 123;
        	iMode1 = 121;
        	//iMode2 = 122;
	}
	else if(portnum == 2)
	{
        	iMode0 = 134;
        	iMode1 = 136;
        	//iMode2 = 135;
	}
        else
	{
        	//print_help();
               return;
	}

	//set_gpio(iMode2,0);
	if(mode== 232)
	{
		set_gpio(iMode0,1);
		set_gpio(iMode1,0);
	}
	else if(mode == 422)
	{
		set_gpio(iMode0,1);
		set_gpio(iMode1,1);
	}
	else if(mode == 485)
	{
		set_gpio(iMode0,0);
		set_gpio(iMode1,1);
	}
	else
	{
		char buf[200];
		sprintf(buf,"Only support RS232/RS422/RS485 mode.");
	}  
}



void switch_mode(int com , int mode)
{
     char fn[50];
     int fd;
     struct termios T_new;

     switch(com)
     {
      case 1:
           sprintf(fn,"/dev/ttymxc3");
           break;
      case 2:
           sprintf(fn,"/dev/ttymxc2");
           break;
      
     }

     fd = open(fn, O_RDWR | O_NOCTTY | O_NDELAY);
     switch(mode)
     {
     	case 232:
	     tcgetattr(fd, &T_new);
	     T_new.c_cflag |= CRTSCTS; 
	     tcsetattr(fd, TCSANOW, &T_new);
	     chane_mode_232(fd);
	     close(fd);
             break;
       case 422:
       case 485:
	     tcgetattr(fd, &T_new);
	     T_new.c_cflag &= ~CRTSCTS; 
	     tcsetattr(fd, TCSANOW, &T_new);
	     chane_mode_485_422(fd);
	     close(fd);
             break;
     }
     change_io_sta(com,mode);

     printf("Switch COM%d(%s) to %d mode\n",com,fn,mode);
     

     return;
}

void print_help()
{
     printf("Example: EIC-2000_setcom.exe com mode\n");
     printf("COM  => 1/2\n");
     printf("mode => 232/485\n");
     printf("COM1 => /dev/ttymxc3\n");
     printf("COM2 => /dev/ttymxc2\n");
     
     
}


int main(int argc, char	*argv[])
{
    int com = 0; // 
    int mode = 0; // 232 / 485 / 422
    unsigned char flag_error = 0;
    
    
    if(argc!=3)
    {
     goto error;
    }
    
    com = atoi(argv[1]);
    if(com < 1 || com >2)
    {
      goto error;
    }
    
    mode = atoi(argv[2]);
    if((mode != 232)&& (mode != 422)&& (mode != 485) )
    {
      goto error;
    }
    
    switch_mode(com,mode);
    
    return 0;
    
    error:
    	print_help();
    return 0;
    
}

