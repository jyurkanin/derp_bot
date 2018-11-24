#include <stdio.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <unistd.h>
#include <ros/ros.h>
#include <std_msgs/String.h>
#include <sstream>

typedef struct {
    unsigned int distance;
    unsigned int strength;
} lidar_t;

int measure(int fd, lidar_t *packet);

int main(void){
    char *lidar = "/dev/ttyS0";
    int fd;
    lidar_t packet;
    wiringPiSetup();
    fd = serialOpen(lidar, 115200);
    while(1){
        usleep(1000);
//        printf("%d\n", serialGetchar(fd));
        if(measure(fd, &packet)){
            printf("Distance %d\t\tStrength %d\n", packet.distance, packet.strength);
        }
    }
    serialClose(fd);
}

int measure(int fd, lidar_t *packet){
    unsigned char TFbuff[9] = {0};
    long checksum  = 0;
    TFbuff[0] = serialGetchar(fd);
    checksum += TFbuff[0];
    if(TFbuff[0] == 'Y'){
        TFbuff[1] = serialGetchar(fd);
        checksum += TFbuff[1];
        if(TFbuff[1] == 'Y'){
            for(int i = 2;i < 8;i++){
                TFbuff[i] = serialGetchar(fd);
                checksum += TFbuff[i];
            }
            TFbuff[8] = serialGetchar(fd);
            checksum &= 0xff;
            if(checksum == TFbuff[8]){
                packet->distance = TFbuff[2]+TFbuff[3]*256;
                packet->strength = TFbuff[4]+TFbuff[5]*256;
                return 1;
            }else{
                checksum  = 0;
                return 0;
            }
        }else{
            checksum  = 0;
            return 0;
        }
    }
    else{
        checksum  = 0;
        return 0;
    }
}
