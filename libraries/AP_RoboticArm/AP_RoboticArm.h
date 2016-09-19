#ifndef AP_ROBOTICARM_H
#define AP_ROBOTICARM_H



#include <nuttx/config.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h> 		   /* File Control Definitions      	*/
#include <termios.h>   	 /* POSIX Terminal Control Definitions*/
#include <unistd.h>    	 /* UNIX Standard Definitions     	*/

class RoboticArm{
  public:
    bool deployArm ()
    {
      int fd,state;
      const char *device = "/dev/ttyS6";   /* The port where the arduino is */
      const char *message = "start";
      fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY);
      if(fd == -1) {   										 /* If something went wrong*/
          return false;
      }
      else {   										       	 /* If everything went fine */
          state = write(fd,message,sizeof("start"));
      }
      if ( state == -1){
          return false;
      }
      return true;
    }
};

#endif  // AP_ROBOTICARM_H
