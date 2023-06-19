/*
 * @author Copyright (c) 2023 Martin Oberzalek
 */

#include "dht22.h"
#include <main_tasks.h>
#include <cmsis_os.h>
#include <stdio.h>
#include <main.h>
#include "SimpleOutDebug.h"
#include <format.h>
#include <string_utils.h>
#include "Measure.h"
#include "MeasureResult.h"

using namespace Tools;


void measureInsideTask( TIM_HandleTypeDef * phtim1 )
{
	SimpleOutDebug out_debug;
	Tools::x_debug = &out_debug;

	Measure task( WHERE::INSIDE, GPIO_DHT22_INSIDE_GPIO_Port, GPIO_DHT22_INSIDE_Pin, *phtim1 );
	task.run();
}

void mesureOutsideTask( TIM_HandleTypeDef * phtim1 )
{
	Measure task( WHERE::OUTSIDE, GPIO_DHT22_OUTSIDE_GPIO_Port, GPIO_DHT22_OUTSIDE_Pin, *phtim1 );
	task.run();
}

void calculateResultsTask()
{
	auto & mr = MeasureResult::instance();

	do {
		try {
			osDelay(10000);
			DEBUG( __FUNCTION__ );
			mr.getAccumulatedResult();
		} catch( const std::exception & error ) {
			DEBUG( format( "Error: %s", error.what() ));
		}
	} while( true );
}
