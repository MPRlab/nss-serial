#include "mbed.h"

/*DigitalOut solenoid[] = {(PG_1 ),(PF_7 ),(PE_3 ),(PE_2 ),
                         (PD_6 ),(PD_2 ),(PG_0 ),(PD_0 ),
                         (PE_6 ),(PD_3 ),(PD_7 ),(PC_12),
                         (PD_1 ),(PF_8 ),(PE_5 ),(PD_4 ),
                         (PG_3 ),(PC_11),(PF_9 ),(PF_0 ), //PF_0 NOT PH_0
                         (PE_4 ),(PD_5 ),(PG_2 ),(PC_10),
                         (PC_9 )};
*/

DigitalOut solenoid[] = {(LED1), (LED2), (LED3)};

Serial pc(USBTX, USBRX); // set tx, rx for serial object "pc"

const char buffSize = 10;
char inputBuffer[buffSize];
char bytesRcvd = 0;
const static char startMarker = 128;
const static char endMarker = 129;
bool readInProgress = false;

void noteOnOff(int pitch, int vel) {
   if (vel > 0) {
       solenoid[pitch] = 1;
   }
   if (vel == 0) {
       solenoid[pitch] = 0;
   }
}

void read_serial() {

   if(pc.readable()) { // true when there is something available to read.

       char x = pc.getc();
       pc.putc(x);

       if(x == endMarker) {
           readInProgress = false;
           const char pitch = (inputBuffer[0] - 1);
           const char vel = inputBuffer[1];
           inputBuffer[bytesRcvd] = 0;
           noteOnOff(pitch, vel);
       }

       if(readInProgress) {
           inputBuffer[bytesRcvd] = x;
           bytesRcvd ++;
       }
       if(x == startMarker){
           bytesRcvd = 0;
           readInProgress = true;
       }
   }
}

int main() {
   pc.baud(115200); //set the baud rate
   while(1){
       read_serial();
   }
}
