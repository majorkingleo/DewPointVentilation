/*
 * main_tasks.cc
 *
 *  Created on: 26.04.2023
 *      Author: martin
 */
#include "dht22.h"
#include <main_tasks.h>
#include <cmsis_os.h>
#include <stdio.h>
#include <main.h>
#include "SimpleOutDebug.h"
#include <format.h>
#include <string_utils.h>
#include <math.h>
#include "Measure.h"

using namespace Tools;

// calculates dewpoint with magnus formular
float dewpoint( float temp_celsius, float humidity )
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


void measureInsideTask( TIM_HandleTypeDef * phtim1 )
{
	SimpleOutDebug out_debug;
	Tools::x_debug = &out_debug;

	Measure task( "INSIDE ", GPIO_DHT22_INSIDE_GPIO_Port, GPIO_DHT22_INSIDE_Pin, *phtim1 );
	task.run();
}

void mesureOutsideTask( TIM_HandleTypeDef * phtim1 )
{
	Measure task( "OUTSIDE", GPIO_DHT22_OUTSIDE_GPIO_Port, GPIO_DHT22_OUTSIDE_Pin, *phtim1 );
	task.run();
}
