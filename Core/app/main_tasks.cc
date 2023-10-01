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

#warning "DEBUG correction for testing set!!!!"
	Measure task( WHERE::INSIDE, GPIO_DHT22_INSIDE_GPIO_Port, GPIO_DHT22_INSIDE_Pin, *phtim1, 5, 5 );
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

	const unsigned MIN_HUMIDITY_DIFF = 1;
	const unsigned MIN_DEWPOINT_DIFF = 2;

	do {
		try {
			osDelay(10000);
			// DEBUG( __FUNCTION__ );
			auto result = mr.getAccumulatedResult();

			if( !result ) {
				continue;
			}

			auto & inside = std::get<MeasureResult::RESULT_DATA_INSIDE>(result.value());
			auto & outside = std::get<MeasureResult::RESULT_DATA_OUTSIDE>(result.value());

			float hdiff = inside.humidity - outside.humidity;

			if( hdiff <= MIN_HUMIDITY_DIFF ) {
				continue;
			}

			if( inside.dewpoint <= outside.dewpoint ) {
				continue;
			}

			float ddiff = inside.dewpoint - outside.dewpoint;

			if( ddiff <= MIN_DEWPOINT_DIFF ) {
				continue;
			}

			CPPDEBUG( "start fan" );

		} catch( const std::exception & error ) {
			CPPDEBUG( format( "Error: %s", error.what() ));
		}
	} while( true );
}
