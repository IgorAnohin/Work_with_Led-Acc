CC = g++
NAVIO = ../Navio/C++/Navio
INCLUDES = -I ../Navio/C++

all:
	$(CC) $(INCLUDES) work.cpp $(NAVIO)/PCA9685.cpp $(NAVIO)/I2Cdev.cpp $(NAVIO)/gpio.cpp $(NAVIO)/Util.cpp  $(NAVIO)/MPU9250.cpp -o must_work

clean:
	rm must_work



