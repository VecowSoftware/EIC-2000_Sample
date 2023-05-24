#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <libgen.h>
#include <time.h>    // time()
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/uio.h>
#include <linux/can.h>
#include <net/if.h>
#include <linux/can/raw.h>

#define PF_CAN 29

#define LOOP 100

#define CAN_ID_DEFAULT	(2)

int init_can(char *name) 
{
	int skt;
	struct ifreq ifr;
	struct sockaddr_can addr;
	
	if ((skt = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) 
	{
		perror("socket");
		return -1;
	}

	addr.can_family = PF_CAN;
	strcpy(ifr.ifr_name, name);
	ioctl(skt, SIOCGIFINDEX, &ifr);
	addr.can_ifindex = ifr.ifr_ifindex;
	
	if (bind(skt, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("bind");
		return -1;
	}
	return skt;
}

int can_send(int s1, struct can_frame frame)
{
    int ret;
    
    ret = write(s1, &frame, sizeof(frame));
    if(ret < 0 ) {
        perror("can write");
        return -1;
    }
    return 0;
}

int can_read(int s2, struct can_frame frame)
{
    struct can_frame frame2;
    int ret, i, nbytes;
    
    memset(frame2.data , 0 , sizeof(frame2.data));
    if ((nbytes = read(s2, &frame2, sizeof(frame2))) < 0) {
        perror("can read");
        return -1;
    }
    
    //filter
    /*
    if(frame2.can_id != frame.can_id) 
    {
        printf("\nSend : [%03x] %s\n" ,frame.can_id, frame.data);
        printf("Recv : [%03x] %s\n", frame2.can_id, frame2.data);
        return -1;
    }
    else if(strcmp(frame2.data, frame.data) != 0)
    {
        printf("\nSend : [%03x] %s\n" ,frame.can_id, frame.data);
        printf("Recv : [%03x] %s\n", frame2.can_id, frame2.data);
        return -1;
    }*/ 
    printf("Recv: [%03x] %s\n", frame2.can_id, frame2.data);
}

int main(int argc, char **argv)
{
    //argv[1] = 0 or 1  for can0 or can1
	static int skt = -1 , skt2 = -1, extended = 0;
	int err = 0, j, id, loop;
	unsigned int maxid;
	char ifc[20];
    
	struct can_frame frame = {
		.can_dlc = 1,
	};
    
	struct can_filter filter[] = {
		{
			.can_id = 2,
		},
	};
	if(argc < 2){return 0;}
	
	//init canbus sokcet
	sprintf(ifc,"can%d",atoi(argv[1]));
	printf("%s\n",ifc);
	skt = init_can(ifc);
	if(skt < 0) {
		printf("Error Init can\n",atoi(argv[1]));
		return 0;
	}
	/*
	skt2 = init_can("can1");
	if(skt2 < 0) {
		printf("Error Init can1\n");
		return 0;
	}*/
    
    
    //send message
    id = 0x7d1+ atoi(argv[1]);
    filter->can_id = id;
    if (extended) {
			filter->can_mask = CAN_EFF_MASK;
			filter->can_id  &= CAN_EFF_MASK;
			filter->can_id  |= CAN_EFF_FLAG;
    } 
    else {
			filter->can_mask = CAN_SFF_MASK;
			filter->can_id  &= CAN_SFF_MASK;
    }
    frame.can_id = filter->can_id;
    strcpy((char *)frame.data, "12345678");
    frame.can_dlc = strlen(frame.data);
    printf("Send: [%03x] %s %d\n", frame.can_id, frame.data, frame.can_dlc);
    can_send(skt, frame);
    
    
    //read message
    while(1)
    {
     usleep(100000);
     can_read(skt, frame);
     
     }
    
	close(skt);
	//close(skt2);

	return 0;
}
