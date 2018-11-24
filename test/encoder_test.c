#include <stdio.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>
#include <inttypes.h>

#define GEAR_RATIO 80/12.0
#define MAX_COUNT 0 //?
#define MOTOR_PIN 23
#define ENCODER_PIN 18


int encoder_count = 0;
void encoder_isr(void){
    encoder_count++;
//    if(encoder_count > )
}

int main(void){
    wiringPiSetupGpio();
    pinMode(ENCODER_PIN, INPUT);
    pullUpDnControl(ENCODER_PIN, PUD_DOWN);
    wiringPiISR(ENCODER_PIN, INT_EDGE_RISING, encoder_isr);
    
    while(1){
        printf("%d\n", encoder_count);
    }
}

