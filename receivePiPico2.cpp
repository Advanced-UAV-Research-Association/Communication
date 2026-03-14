#include <iostream>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#define HEADER 0xAA

int main()
{
    int serial_port = open("/dev/serial0", O_RDWR);

    struct termios tty;
    tcgetattr(serial_port, &tty);

    cfsetispeed(&tty, B115200);
    cfsetospeed(&tty, B115200);

    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;

    tcsetattr(serial_port, TCSANOW, &tty);

    uint8_t buffer[14];

    while(true)
    {
        read(serial_port, buffer, 14);

        if(buffer[0] != HEADER)
            continue;

        int16_t ax = buffer[2]<<8 | buffer[1];
        int16_t ay = buffer[4]<<8 | buffer[3];
        int16_t az = buffer[6]<<8 | buffer[5];

        int16_t gx = buffer[8]<<8 | buffer[7];
        int16_t gy = buffer[10]<<8 | buffer[9];
        int16_t gz = buffer[12]<<8 | buffer[11];

        std::cout << "Accel: "
                  << ax << " "
                  << ay << " "
                  << az << std::endl;

        std::cout << "Gyro: "
                  << gx << " "
                  << gy << " "
                  << gz << std::endl;
    }
}