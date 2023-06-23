/*
 * @author Copyright (c) 2023 Martin Oberzalek
 */

#ifndef SRC_MEASURERESULT_H_
#define SRC_MEASURERESULT_H_

#include "dht22.h"
#include <tuple>
#include "CyclicArray.h"
#include <OsMutex.h>

enum class WHERE
{
	UNDEFINED,
	INSIDE,
	OUTSIDE
};

class MeasureResult
{
public:
	struct Result : public DHT22::Result
	{
		float dewpoint = std::numeric_limits<float>::infinity();;
		bool valid = false;
		WHERE where = WHERE::UNDEFINED;

		Result() = default;

		Result( WHERE where,
				DHT22::Result );

		Result( WHERE where,
				float tempCelsius,
				float tempFahrenheit,
				float humidity );

		void recalcDewpoint();
	};

protected:
	typedef std::tuple<Result,Result> RESULT_DATA;

	CyclicArray<RESULT_DATA,20> buffer;
	OsMutex 					m_buffer;
	bool						show_diff; // show difference in temp °C for calibration

public:
	MeasureResult();

	void addMeasureResult( const Result & result );

	static float dewpoint( float temp_celsius, float humidity );

	static MeasureResult & instance();

	std::optional<RESULT_DATA> getAccumulatedResult();

protected:
	template<size_t N> Result & getLastUndefined();
	template<size_t N> void accumulate( RESULT_DATA & accumulated_data, const RESULT_DATA & data );
	void calcAvarage( Result & accumulated_data, float valid_measures );
};



#endif /* SRC_MEASURERESULT_H_ */
