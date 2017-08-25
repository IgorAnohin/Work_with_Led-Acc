/*
Provided to you by Emlid Ltd (c) 2014.
twitter.com/emlidtech || www.emlid.com || info@emlid.com

Example: Control RGB LED with PCA9685 driver onboard of Navio shield for Raspberry Pi.

RGB LED is connected to 0,1,2 channels of PWM controller PCA9685.
As channels are connected to LED's cathodes the logic is inverted,
that means that 0 value sets max brightness and 4095 sets min brightness.

To run this example navigate to the directory containing it and run following commands:
make
./LED
*/

#include <Navio/gpio.h>
#include "Navio/Util.h"
#include "Navio/PCA9685.h"
#include "Navio/MPU9250.h"
#include <stdlib.h>

using namespace Navio;

enum {
    Yellow,
    Cyan,
    Magneta,
    Blue,
    Red,
    Green
};


void set_color(int color)
{
    PCA9685 pwm;
    switch( color )
    {
        case Yellow:
            pwm.setPWM(2, 0);
            pwm.setPWM(1, 0);
            pwm.setPWM(0, 4095);
            printf("LED is Yellow\n");
        break;

        case Cyan:
            pwm.setPWM(2, 4095);
            pwm.setPWM(1, 0);
            pwm.setPWM(0, 0);
            printf("LED is Cyan\n");
        break;

        case Magneta:
            pwm.setPWM(2, 0);
            pwm.setPWM(1, 4095);
            pwm.setPWM(0, 0);
            printf("LED is Magneta\n");
        break;

        case Blue:
            pwm.setPWM(2, 4095);
            pwm.setPWM(1, 4095);
            pwm.setPWM(0, 0);
            printf("LED is Blue\n");
        break;

        case Red:
            pwm.setPWM(2, 0);
            pwm.setPWM(1, 4095);
            pwm.setPWM(0, 4095);
            printf("LED is Red\n");
        break;

        case Green:
            pwm.setPWM(2, 4095);
            pwm.setPWM(1, 0);
            pwm.setPWM(0, 4095);
            printf("LED is Green\n");
        break;


    }


}


void get_acc(MPU9250 imu,float *ax, float *ay, float *az)
{
    imu.read_acc();
    *ax = imu.accelerometer_data[0];
    *ay = imu.accelerometer_data[1];
    *az = imu.accelerometer_data[2];
}


int Get_deviation(float accel)
{
    if (accel > 8) return 1; //+g
        else if (accel < -8) return -1; //-g
                else return 0; //g=0
}



int main()
{
    static const uint8_t outputEnablePin = RPI_GPIO_27;

    if (check_apm()) {
        return 1;
    }

    Pin pin(outputEnablePin);

    if (pin.init()) {
        pin.setMode(Pin::GpioModeOutput);
        pin.write(0); /* drive Output Enable low */
    } else {
        fprintf(stderr, "Output Enable not set. Are you root?\n");
        return 1;
    }

    PCA9685 pwm;

    if (check_apm()) {
        return 1;
    }

    //-------------------------------------------------------------------------
        MPU9250 imu;
        imu.initialize();

        float ax, ay, az;
        int X = 0, Y = 0, Z = 0;

    //-------------------------------------------------------------------------


    pwm.initialize();

    while (true) {

        get_acc(imu,&ax, &ay, &az);

        X = Get_deviation(ax);
        Y = Get_deviation(ay);
        Z = Get_deviation(az);

        if (X == 0 && Y == 0 && Z == 1)
            //Neutral position
            set_color(Yellow);

        if (X == 1 && Y == 0 && Z == 0)
            //Left
            set_color(Cyan);

        if (X == -1 && Y == 0 && Z == 0)
            //right
            set_color(Magneta);

        if (X == 0 && Y == 1 && Z == 0)
            //Back
            set_color(Blue);

        if (X == 0 && Y == -1 && Z == 0)
            //forward
            set_color(Red);

        if (X == 0 && Y == 0 && Z == -1)
            //up down
            set_color(Green);

        sleep(1);
    }

    return 0;
}
