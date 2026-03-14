#include <wire.h>
#include <MPU6050.h>

MPU6050 mpu;

#define HEADER 0xAA 
#define PACKET_SIZE 14 

uint8_t packet[PACKET_SIZE];

uint8_t computeChecksum(uint8_t *data, int len)
{
    uint8_t sum = 0; 
    for(int i = 0; i < len; i++)
        sum += data[i];
    return sum;
}

void setup() 
{
    Serial.begin(115200); 

    Wire.begin();  // I2C Communication begins.

    mpu.initialize(); // mpu initialization

    if(!mpu.testConnection())
    {
        while(1)
        {
            // Stop if sensor fails
        }
    }
}

void loop()
{
    int16_t ax, ay, az;  // Initializing the accelerometer values at x,y,z axes.
    int16_t gx, gy, gz;  // Initializing the gyroscope values at x,y,z axes.

    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz); // Accelerometer and gyroscope values

    packet[0] = HEADER; 

    packet[1] = ax & 0xFF;
    packet[2] = ax >> 8;

    packet[3] = ay & 0xFF;
    packet[4] = ay >> 8;

    packet[5] = az & 0xFF;
    packet[6] = az >> 8;

    packet[7] = gx & 0xFF;
    packet[8] = gx >> 8;

    packet[9] = gy & 0xFF;
    packet[10] = gy >> 8;

    packet[11] = gz & 0xFF;
    packet[12] = gz >> 8;

    packet[13] = computeChecksum(packet, 13);

    Serial.write(packet, PACKET_SIZE);

    delay(10);
}
