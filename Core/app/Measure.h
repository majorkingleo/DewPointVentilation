/*
 * @author Copyright (c) 2023 Martin Oberzalek
 */


#ifndef SRC_MEASURE_H_
#define SRC_MEASURE_H_

#include <dht22.h>
#include "MeasureResult.h"

class Measure
{
	DHT22HAL dht22;
	int16_t measureDelay = 1000;
	WHERE where;

public:
	Measure( WHERE					where,
			 GPIO_TypeDef			*gpioPort,
			 uint16_t				gpioPin,
			 TIM_HandleTypeDef & 	htim,
			 float 					degree_correction = 0,
			 float 					humidity_correction = 0 );

	void run();

	static const char* toString( WHERE where );
};




#endif /* SRC_MEASURE_H_ */
