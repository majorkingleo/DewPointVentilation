/*
 * @author Copyright (c) 2023 Martin Oberzalek
 */

#include "MeasureResult.h"
#include <math.h>
#include "SimpleOutDebug.h"
#include <format.h>

using namespace Tools;

MeasureResult::Result::Result( WHERE where_,
							   float tempCelsius,
							   float tempFahrenheit,
							   float humidity )
: DHT22::Result( tempCelsius, tempFahrenheit, humidity ),
  where( where_ )
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
	dewpoint = MeasureResult::dewpoint( tempCelsius, tempFahrenheit );
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
			return std::get<0>( data );
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

	{
		std::lock_guard<OsMutex> lock(m_buffer);

		for( auto & data : buffer ) {
			auto & inside = std::get<0>(data);
			auto & outside = std::get<1>(data);

			if( inside.where == WHERE::INSIDE &&
					outside.where == WHERE::OUTSIDE &&
					inside.valid &&
					outside.valid ) {
				valid_buffer.push_back( data, true );
			}
		}
	}

	if( valid_buffer.empty() ) {
		DEBUG( "valid data buffer is empty");
		return ret;
	}

	DEBUG( format("valid data buffer size: %d", valid_buffer.size()) );

	RESULT_DATA accumulated_data;

	for( auto & data : valid_buffer ) {
		accumulate<0>(accumulated_data,data);
		accumulate<1>(accumulated_data,data);
	}

	const float valid_measures = valid_buffer.size();

	calcAvarage( std::get<0>(accumulated_data), valid_measures );
	calcAvarage( std::get<1>(accumulated_data), valid_measures );

	DEBUG( format( "Dewpoint inside: %.02f outside: %.02f measures: %d",
			std::get<0>(accumulated_data).dewpoint,
			std::get<1>(accumulated_data).dewpoint,
			valid_measures ) );

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

