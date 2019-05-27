#ifndef ISA_SPEED_SENSOR_H
#define ISA_SPEED_SENSOR_H

/*
 * Silniki, wejście enkoderowe
 *
 */
#define ENCODER_LEFT	50	// Enkoder lewej strony
#define ENCODER_RIGHT	51	// Enkoder prawej strony	

void initEncoder();
class SpeedSensor {
    SpeedSensor();
public:
    static SpeedSensor& getInstance(); 
};

#endif