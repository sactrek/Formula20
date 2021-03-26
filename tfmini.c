/* ----------------------------------------------------------------------- * 
 * Title:         tfmini.c                                                 * 
 * Description:   C-code for TFMini Plus                                   * 
 *                Tested on BeagleBone AI                                  * 
 *                11/6/2019 Sean J. Miller                                 * 
 * Prerequisites:                             * 
 *------------------------------------------------------------------------ */  
#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <linux/i2c-dev.h>  
#include <fcntl.h>  
#include <sys/ioctl.h>  
#include <string.h>  
#define I2C_BUS        "/dev/i2c-3" // I2C bus device  
#define I2C_ADDR       0x10         // I2C slave address for the TFMini module  
int debug;  
int i2cFile;  
void i2c_start() {  
   if((i2cFile = open(I2C_BUS, O_RDWR)) < 0) {  
      printf("Error failed to open I2C bus [%s].\n", I2C_BUS);  
      exit(-1);  
   } else {  
       if (debug)printf("Opened I2C Bus\n");  
   }  
   // set the I2C slave address for all subsequent I2C device transfers  
   if (ioctl(i2cFile, I2C_SLAVE, I2C_ADDR) < 0) {  
      printf("Error failed to set I2C address [%s].\n", I2C_ADDR);  
      exit(-1);  
   } else {  
      printf("Set Slave Address\n");  
   }  
}  
void i2c_stop() { close(i2cFile); }  
float readDistance() {  
 //Routine to output the distance to the console  
 int distance = 0; //distance  
 int strength = 0; // signal strength  
 int rangeType = 0; //range scale  
 unsigned char incoming[7]; //an array of bytes to hold the returned data from the TFMini.  
 unsigned char cmdBuffer[] = { 0x01, 0x02, 7 }; //the bytes to send the request of distance  
   
   write( i2cFile, cmdBuffer, 3 );  
   usleep(100000);  
   read(i2cFile, incoming, 7);  
 for (int x = 0; x < 7; x++)  
 {  
  if (x == 0)  
  {  
   //Trigger done  
   if (incoming[x] == 0x00)  
   {  
     
   }  
   else if (incoming[x] == 0x01)  
   {  
     
   }  
  }  
  else if (x == 2)  
   distance = incoming[x]; //LSB of the distance value "Dist_L"  
  else if (x == 3)  
   distance |= incoming[x] << 8; //MSB of the distance value "Dist_H"  
  else if (x == 4)  
   strength = incoming[x]; //LSB of signal strength value  
  else if (x == 5)  
   strength |= incoming[x] << 8; //MSB of signal strength value  
  else if (x == 6)  
   rangeType = incoming[x]; //range scale  
 }  
   
 float the_return = distance / (12 * 2.54); //convert to feet.  
 return the_return;  
}  
void main() {  
   i2c_start();   
   debug=1; //change to 1 to see messages.  
  
   if(debug) printf("Init Start:\n");  
   float my_distance = readDistance();  
   if(debug) printf("the_distance: %f\n",my_distance);  
   if(debug) printf("Finished reading.\n");  
   i2c_stop();  
   if(debug) printf("Finshed, Sean!\n\n");  
} 