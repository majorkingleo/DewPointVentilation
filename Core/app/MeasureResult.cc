/*
 * @author Copyright (c) 2023 Martin Oberzalek
 */

#include "MeasureResult.h"
#include <math.h>
#include "SimpleOutDebug.h"
#include <format.h>
#include "Measure.h"

using namespace Tools;
using namespace std::chrono_literals;
using namespace std::chrono;

static MeasureResult::Result ZERO = { WHERE::UNDEFINED, 0.0, 0.0, 0.0 };

MeasureResult::Result::Result( WHERE where_,
							   float tempCelsius,
							   float tempFahrenheit,
							   float humidity )
: DHT22::Result( tempCelsius, tempFahrenheit, humidity ),
  where( where_ ),
  measure_time_point(clock::now())
{

	recalcDewpoint();
}

MeasureResult::Result::Result( WHERE where_,
						      DHT22::Result res )
: Result( where_, res.tempCelsius, res.tempFahrenheit, res.humidity )
{

}

void MeasureResult::Result::recalcDewpoint()
{
	 constexpr auto inf = std::numeric_limits<float>::infinity();

	 if( tempCelsius != inf &&
		 humidity != inf ) {

		 dewpoint = MeasureResult::dewpoint( tempCelsius, humidity );

		 if( dewpoint != inf ) {
			 valid = true;
		 }
	 } else {
		 valid = false;
	 }

	 if( measure_time_point == measure_time_point_default ) {
		 valid = false;
	 }
}

MeasureResult::MeasureResult()
: show_diff( true )
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

	// dewpoint temp (�C)
	float dewpoint = ( b * vp_log ) / ( a - vp_log );
	return dewpoint;
}

MeasureResult & MeasureResult::instance()
{
	static MeasureResult result;
	return result;
}

void MeasureResult::addMeasureResult( const Result & result )
{
	std::lock_guard<OsMutex> lock(m_buffer);

	Result *res = nullptr;

	if( result.where == WHERE::INSIDE ) {
		res = &getLastUndefined<0>();
	} else {
		res = &getLastUndefined<1>();
	}

	*res = result;
	// res->recalcDewpoint();
}

template<size_t N>
MeasureResult::Result & MeasureResult::getLastUndefined()
{
	RESULT_DATA *res = nullptr;

	if( buffer.empty() ) {
		buffer.push_back( RESULT_DATA() );
		res = &buffer.front();
	} else {
		auto & data = buffer.at(buffer.size()-1);

		if( std::get<N>( data ).where == WHERE::UNDEFINED ) {
			return std::get<N>( data );
		} else {
			buffer.push_back( RESULT_DATA(), true );
			res = &buffer.front();
		}
	}


	return std::get<N>( *res );
}

std::optional<MeasureResult::RESULT_DATA> MeasureResult::getAccumulatedResult()
{
	std::optional<RESULT_DATA> ret;
	CyclicArray<RESULT_DATA,10> valid_buffer;

	const auto results_not_older_than = Result::clock::now() - 10min;

	{
		std::lock_guard<OsMutex> lock(m_buffer);

		for( auto & data : buffer ) {
			auto & inside = std::get<RESULT_DATA_INSIDE>(data);
			auto & outside = std::get<RESULT_DATA_OUTSIDE>(data);

			/*
			DEBUG( format( "inside: %s outside: %s %s %s",
					Measure::toString(inside.where),
					Measure::toString(outside.where),
					inside.valid ? "v" : "n",
					outside.valid ? "v" : "n" ) );
			*/

			if( inside.where == WHERE::INSIDE &&
					outside.where == WHERE::OUTSIDE &&
					inside.valid &&
					outside.valid ) {

				if( inside.measure_time_point >= results_not_older_than &&
					outside.measure_time_point >= results_not_older_than ) {

					/*
					auto diff = inside.measure_time_point - results_not_older_than;
					auto diff_in_sec = std::chrono::duration_cast<std::chrono::seconds>(diff).count();

					DEBUG( format( "diff: %d", diff_in_sec ) );
					*/

					valid_buffer.push_back( data, true );
				}
			}
		}
	}

	if( valid_buffer.empty() ) {
		CPPDEBUG( "valid data buffer is empty");
		return ret;
	}

	// DEBUG( format("valid data buffer size: %d", valid_buffer.size()) );

	RESULT_DATA accumulated_data = std::make_tuple( ZERO, ZERO );

	for( auto & data : valid_buffer ) {
		accumulate<RESULT_DATA_INSIDE>(accumulated_data,data);
		accumulate<RESULT_DATA_OUTSIDE>(accumulated_data,data);
	}

	const float valid_measures = valid_buffer.size();

	calcAvarage( std::get<RESULT_DATA_INSIDE>(accumulated_data), valid_measures );
	calcAvarage( std::get<RESULT_DATA_OUTSIDE>(accumulated_data), valid_measures );

	if( show_diff ) {
		CPPDEBUG( format( "Difference in °C between both sensors: %0.2f°",
				std::get<RESULT_DATA_INSIDE>(accumulated_data).tempCelsius -
				std::get<RESULT_DATA_OUTSIDE>(accumulated_data).tempCelsius ) );
	}

	CPPDEBUG( format( "Accumulated dewpoint inside: %.02f outside: %.02f measures: %d",
			std::get<RESULT_DATA_INSIDE>(accumulated_data).dewpoint,
			std::get<RESULT_DATA_OUTSIDE>(accumulated_data).dewpoint,
			static_cast<int>(valid_measures) ) );

	ret = accumulated_data;

	return ret;
}

template<size_t N>
void MeasureResult::accumulate( RESULT_DATA & accumulated_data, const RESULT_DATA & data )
{
	std::get<N>(accumulated_data).tempCelsius += std::get<N>(data).tempCelsius;
	std::get<N>(accumulated_data).tempFahrenheit += std::get<N>(data).tempFahrenheit;
	std::get<N>(accumulated_data).humidity += std::get<N>(data).humidity;
}

void MeasureResult::calcAvarage( Result & accumulated_data, float valid_measures )
{
	accumulated_data.tempCelsius /= valid_measures;
	accumulated_data.tempFahrenheit /= valid_measures;
	accumulated_data.humidity /= valid_measures;
	accumulated_data.recalcDewpoint();
}

