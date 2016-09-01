#ifndef AP_ROBOTICARM_H
#define AP_ROBOTICARM_H



#include <nuttx/config.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h> 		 /* File Control Definitions      	*/
#include <termios.h>   	 /* POSIX Terminal Control Definitions*/
#include <unistd.h>    	 /* UNIX Standard Definitions     	*/

class RoboticArm{
  public:
    bool deployArm ()
    {
      int fd;
      const char *device = "/dev/ttyS6";
      fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY);
      if(fd == -1) {   										 /* If something went wrong*/
          return false;
      }
      else {   										       	 /* If everything went fine */
          write(fd,"1",1);
      }
      return true;
    }
};

#endif  // AP_PERFMON_H
