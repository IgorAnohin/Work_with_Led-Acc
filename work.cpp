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

enum {
     Neutral,
     Left,
     Right,
     Back,
     Forward,
     Invert
};


static void set_color(PCA9685 *pwm,int color)
{
    switch( color )
    {
        case Yellow:
            pwm->setPWM(2, 0);
            pwm->setPWM(1, 0);
            pwm->setPWM(0, 4095);
            printf("LED is Yellow\n");
        break;

        case Cyan:
            pwm->setPWM(2, 4095);
            pwm->setPWM(1, 0);
            pwm->setPWM(0, 0);
            printf("LED is Cyan\n");
        break;

        case Magneta:
            pwm->setPWM(2, 0);
            pwm->setPWM(1, 4095);
            pwm->setPWM(0, 0);
            printf("LED is Magneta\n");
        break;

        case Blue:
            pwm->setPWM(2, 4095);
            pwm->setPWM(1, 4095);
            pwm->setPWM(0, 0);
            printf("LED is Blue\n");
        break;

        case Red:
            pwm->setPWM(2, 0);
            pwm->setPWM(1, 4095);
            pwm->setPWM(0, 4095);
            printf("LED is Red\n");
        break;

        case Green:
            pwm->setPWM(2, 4095);
            pwm->setPWM(1, 0);
            pwm->setPWM(0, 4095);
            printf("LED is Green\n");
        break;


    }


}


static void get_acc(MPU9250 *imu,float *ax, float *ay, float *az)
{
    imu->read_acc();
    *ax = imu->accelerometer_data[0];
    *ay = imu->accelerometer_data[1];
    *az = imu->accelerometer_data[2];
}


static int Get_deviation(float accel)
{
    if (accel > 8) return 1; //+g
        else if (accel < -8) return -1; //-g
                else return 0; //g=0
}

static int get_changed_position(int x, int y, int z)
{
    if (x == 0 && y == 0 && z == 1) return Neutral;

    if (x == 1 && y == 0 && z == 0) return Left;           //Left 90*

    if (x == -1 && y == 0 && z == 0) return Right;           //Right 90*

    if (x == 0 && y == 1 && z == 0) return Back;            //Back 90*

    if (x == 0 && y == -1 && z == 0) return Forward;          //Forward 90*

    if (x == 0 && y == 0 && z == -1) return Invert;           //Invert 180*
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


    if (check_apm()) {
        return 1;
    }

    //-------------------------------------------------------------------------

        PCA9685 pwm;
        pwm.initialize();

        MPU9250 imu;
        imu.initialize();

        float ax, ay, az;
        int change_X, change_Y, change_Z, position;

    //-------------------------------------------------------------------------

    while (true) {

        get_acc(&imu,&ax, &ay, &az);

        change_X = Get_deviation(ax);
        change_Y = Get_deviation(ay);
        change_Z = Get_deviation(az);

        position = get_changed_position(change_X, change_Y, change_Z);

        switch (position) {

            case Neutral:
                 set_color(&pwm, Yellow);
            break;

            case Left:
                 set_color(&pwm, Cyan);
            break;

            case Right:
                 set_color(&pwm, Magneta);
            break;

            case Back:
                 set_color(&pwm, Blue);
            break;

            case Forward:
                 set_color(&pwm, Red);
            break;

            case Invert:
                 set_color(&pwm, Green);
            break;

        }


        sleep(1);
    }

    return 0;
}
