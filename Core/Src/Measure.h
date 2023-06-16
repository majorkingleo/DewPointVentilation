/*
 * Measure.h
 *
 *  Created on: Jun 16, 2023
 *      Author: Martin Oberzalek
 */

#ifndef SRC_MEASURE_H_
#define SRC_MEASURE_H_

#include <dht22.h>

class Measure
{
	DHT22HAL dht22;
	int16_t measure_delay = 1000;
	const char *name;

public:
	Measure( const char *name,
			 GPIO_TypeDef* gpioPort,
			 uint16_t	gpioPin,
			 TIM_HandleTypeDef & htim );

	void run();
};




#endif /* SRC_MEASURE_H_ */
