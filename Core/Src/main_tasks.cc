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

using namespace Tools;

float taupunkt(float t, float r) {

float a, b;

  if (t >= 0) {
    a = 7.5;
    b = 237.3;
  } else if (t < 0) {
    a = 7.6;
    b = 240.7;
  }

  // Sättigungsdampfdruck in hPa
  float sdd = 6.1078 * pow(10, (a*t)/(b+t));

  // Dampfdruck in hPa
  float dd = sdd * (r/100);

  // v-Parameter
  float v = log10(dd/6.1078);

  // Taupunkttemperatur (°C)
  float tt = (b*v) / (a-v);
  return { tt };
}


void main_task( TIM_HandleTypeDef * phtim1 )
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
			DEBUG( format( "OUTSIDE %04d %04d measure failed", x++ ) );
			// printf( "%04d measure failed\r\n", x++ );
		}
		osDelay(1000);
		// HAL_Delay(1000);
	}
}
