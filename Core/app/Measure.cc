/*
 * @author Copyright (c) 2023 Martin Oberzalek
 */


#include "Measure.h"
#include "SimpleOutDebug.h"
#include <format.h>
#include <cmsis_os.h>
#include <MeasureResult.h>

using namespace Tools;

Measure::Measure( WHERE where_, GPIO_TypeDef* gpioPort, uint16_t	gpioPin, TIM_HandleTypeDef & htim )
: dht22( gpioPort, gpioPin, htim ),
  where( where_ )
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
			MeasureResult::Result mr( where, r );

			CPPDEBUG( format( "%s %04d %.2f� %.2f%% dew point: %.2f� %s",
							toString(where),
							loop_count++,
							r.tempCelsius,
							r.humidity,
							mr.dewpoint,
							mr.valid ? "(valid)" : "") );

			MeasureResult::instance().addMeasureResult(mr);
		} else {
			CPPDEBUG( format( "%s %04d measure failed", toString(where), loop_count++ ) );
		}
		osDelay(measureDelay);
	}
}

const char* Measure::toString( WHERE where )
{
	switch( where )
	{
	case WHERE::UNDEFINED: return "UNDEFINED";
	case WHERE::INSIDE:  return "INSIDE ";
	case WHERE::OUTSIDE: return "OUTSIDE";
	}

	return nullptr;
}

