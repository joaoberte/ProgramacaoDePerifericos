#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>

#include <termios.h>
#include <string.h>


using namespace std;
 
#define IN  0
#define OUT 1
 
#define LOW  0
#define HIGH 1
 
static int GPIOExport(int pin)
{
        #define BUFFER_MAX 3
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	int fd;
 
	fd = open("/sys/class/gpio/export", O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open export for writing!\n");
		return(-1);
	}
 
	bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
	write(fd, buffer, bytes_written);
	close(fd);
	return(0);
}
 
static int GPIOUnexport(int pin)
{
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	int fd;
 
	fd = open("/sys/class/gpio/unexport", O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open unexport for writing!\n");
		return(-1);
	}
 
	bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
	write(fd, buffer, bytes_written);
	close(fd);
	return(0);
}
 
static int GPIODirection(int pin, int dir)
{
	static const char s_directions_str[]  = "in\0out";
 
        #define DIRECTION_MAX 35
	char path[DIRECTION_MAX];
	int fd;
 
	snprintf(path, DIRECTION_MAX, "/sys/class/gpio/gpio%d/direction", pin);
	fd = open(path, O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio direction for writing!\n");
		return(-1);
	}
 
	if (-1 == write(fd, &s_directions_str[IN == dir ? 0 : 3], IN == dir ? 2 : 3)) {
		fprintf(stderr, "Failed to set direction!\n");
		return(-1);
	}
 
	close(fd);
	return(0);
}
 
static int GPIORead(int pin)
{
        #define VALUE_MAX 30
	char path[VALUE_MAX];
	char value_str[3];
	int fd;
 
	snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_RDONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio value for reading!\n");
		return(-1);
	}
 
	if (-1 == read(fd, value_str, 3)) {
		fprintf(stderr, "Failed to read value!\n");
		return(-1);
	}
 
	close(fd);
 
	return(atoi(value_str));
}
 
static int GPIOWrite(int pin, int value)
{
	static const char s_values_str[] = "01";
 
	char path[VALUE_MAX];
	int fd;
 
	snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio value for writing!\n");
		return(-1);
	}
 
	if (1 != write(fd, &s_values_str[LOW == value ? 0 : 1], 1)) {
		fprintf(stderr, "Failed to write value!\n");
		return(-1);
	}
 
	close(fd);
	return(0);
}
 

int config_serial(const char * device, unsigned int baudrate){
    struct termios options;
    int fd;
    
    fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY );
    if (fd < 0)
    {
        /*
        * Could not open the port.
        */
      
        perror("config_serial: Não pode abrir a serial - ");
        return -1;
    }

    fcntl(fd, F_SETFL, 0);

    /*
    * Get the current options for the port...
    */
    tcgetattr(fd, &options);
    
    /* sets the terminal to something like the "raw" mode */
    cfmakeraw(&options);
    
    /*
    * Set the baudrate...
    */
    cfsetispeed(&options, baudrate);
    cfsetospeed(&options, baudrate);

    /*
    * Enable the receiver and set local mode...
    */
    options.c_cflag |= (CLOCAL | CREAD);
    
    /*
     * No parit, 1 stop bit, size 8
     */
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;

    
    /*
     * Clear old settings
     */
    options.c_cflag &= ~CRTSCTS;
    options.c_iflag &= ~(IXON | IXOFF | IXANY);
    
    /* non-caninical mode */
    options.c_lflag &= ~ICANON; 

    /*
    * Set the new options for the port...
    */
    tcsetattr(fd, TCSANOW, &options);
    
    /* configura a tty para escritas e leituras não bloqueantes */
    //fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
    
    return fd;
}


int ButtonPush(int pushbutton){
    if (-1 == GPIOExport(pushbutton))
        return(1);
 
    if (-1 == GPIODirection(pushbutton, OUT))
        return(2);

    if (-1 == GPIOWrite(pushbutton, 1))
        return(3);
 
    if (-1 == GPIODirection(pushbutton, IN))
        return(2);

    return GPIORead(pushbutton);
}


int main(int argc, char *argv[])
{
    int fd;
    char selButton = '0';
    char comando;
    char numSeq1, numSeq2, numSeq3, numSeq4;

    printf("\nTrabalho II Programacao de Perifericos");
    printf("\nJean Carlo Burda");
    printf("\n e Joao Berte");

    fd = config_serial("/dev/ttyAMA0", B9600);
        if(fd<0){
                return 0;
        }

    do {

        if(ButtonPush(23) == 0)
            selButton = '1';

        if(ButtonPush(24) == 0)
            selButton = '2';

        if(ButtonPush(25) == 0)
            selButton = '3';

        if(selButton != '0'){
            write(fd, &selButton, 1);
        }
        
        if(selButton == '3'){
            read(fd, &numSeq1, 1);
            read(fd, &numSeq2, 1);
            read(fd, &numSeq3, 1);
            read(fd, &numSeq4, 1);
            printf("\nQuantidade Sequencia 1: %d", numSeq1);
            printf("\nQuantidade Sequencia 2: %d", numSeq2);
            printf("\nQuantidade Sequencia 3: %d", numSeq3);
            printf("\nQuantidade Sequencia 4: %d", numSeq4);
            printf("\n");
        }

        if(selButton != '0'){
            read(fd, &comando, 1);
        }

        selButton = '0';
    }
    while (1);

    return(0);
}

