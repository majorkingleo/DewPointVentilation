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
	printf( "START INSIDE\r\n" );
/*
	while( true ) {
		printf( "START INSIDE\r\n" );
		osDelay(1000);
	}
*/
	Tools::x_debug = new SimpleOutDebug();

	int x = 0;
	DHT22HAL *pdht22 = new DHT22HAL( GPIO_DHT22_INSIDE_GPIO_Port, GPIO_DHT22_INSIDE_Pin, *phtim1 );

	while(true)
	{
		pdht22->start();
		auto res = pdht22->mesure();

		if( res ) {
			DHT22::Result r = res.value();
			// std::string s = "test";
			// s = substitude( s, "%%", "%" );
			// DEBUG( format("test") );
			DEBUG( format( "INSIDE  %04d %.2f° %.2f%%", x++, r.tempCelsius, r.humidity ) );
			// printf( "%04d %f %%\r\n", x++, r.tempCelsius );
		} else {
			DEBUG( format( "INSIDE  %04d measure failed", x++ ) );
			// printf( "%04d measure failed\r\n", x++ );
		}
		osDelay(1000);
		// printf( "INSIDE\r\n" );
		//HAL_Delay(1000);
	}
}

void mesureOutsideTask( TIM_HandleTypeDef * phtim1 )
{
	printf( "START OUTSIDE\r\n" );
/*
	while( true ) {
		printf( "START OUTSIDE\r\n" );
		osDelay(1000);
	}
*/
	int x = 0;
	DHT22HAL *pdht22 = new DHT22HAL( GPIO_DHT22_OUTSIDE_GPIO_Port, GPIO_DHT22_OUTSIDE_Pin, *phtim1 );

	while(true)
	{
		pdht22->start();
		auto res = pdht22->mesure();

		if( res ) {
			DHT22::Result r = res.value();
			// std::string s = "test";
			// s = substitude( s, "%%", "%" );
			// DEBUG( format("test") );
			DEBUG( format( "OUTSIDE %04d %.2f° %.2f%%", x++, r.tempCelsius, r.humidity ) );
			// printf( "%04d %f %%\r\n", x++, r.tempCelsius );
		} else {
			DEBUG( format( "OUTSIDE %04d measure failed", x++ ) );
			// printf( "%04d measure failed\r\n", x++ );
		}
		osDelay(1000);
		// HAL_Delay(1000);
	}
}
