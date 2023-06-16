/*
 * Measrure.cc
 *
 *  Created on: Jun 16, 2023
 *      Author: Martin Oberzalek
 */

#include "Measure.h"
#include "SimpleOutDebug.h"
#include <format.h>
#include <cmsis_os.h>

using namespace Tools;

Measure::Measure( const char *name_, GPIO_TypeDef* gpioPort, uint16_t	gpioPin, TIM_HandleTypeDef & htim )
: dht22( gpioPort, gpioPin, htim ),
  name( name_ )
{

}

void Measure::run()
{
	unsigned loop_count = 0;

	while(true)
	{
		dht22.start();
		auto res = dht22.mesure();

		if( res ) {
			DHT22::Result r = res.value();

			DEBUG( format( "%s %04d %.2f° %.2f%%", name, loop_count++, r.tempCelsius, r.humidity ) );
		} else {
			DEBUG( format( "%s %04d measure failed", name, loop_count++ ) );
		}
		osDelay(measure_delay);
	}
}

