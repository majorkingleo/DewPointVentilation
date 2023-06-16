/*
 * MeasureResult.cc
 *
 *  Created on: Jun 16, 2023
 *      Author: Martin Oberzalek
 */
#include "MeasureResult.h"
#include <math.h>

MeasureResult::Result::Result( WHERE where_,
							   float tempCelsius,
							   float tempFahrenheit,
							   float humidity )
: DHT22::Result( tempCelsius, tempFahrenheit, humidity ),
  where( where_ )
{
	dewpoint = MeasureResult::dewpoint( tempCelsius, tempFahrenheit );
}

MeasureResult::Result::Result( WHERE where_,
						      DHT22::Result res )
: Result( where_, res.tempCelsius, res.tempFahrenheit, res.humidity )
{

}

// calculates dewpoint with magnus formular
float MeasureResult::dewpoint( float temp_celsius, float humidity )
{
	float a = 7.5;
	float b = 237.3;

	if (temp_celsius < 0 ) {
		a = 7.6;
		b = 240.7;
	}

	// saturation vapor pressure (hPa)
	const float svp = 6.1078 * pow(10,( a * temp_celsius ) / ( b + temp_celsius ) );

	// vapor pressure (hPa)
	const float vp = svp * ( humidity / 100.0 );

	const float vp_log = log10( vp / 6.1078 );

	// dewpoint temp (°C)
	float dewpoint = ( b * vp_log ) / ( a - vp_log );
	return dewpoint;
}

MeasureResult & MeasureResult::instance()
{
	static MeasureResult result;
	return result;
}

