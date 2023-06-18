/*
 * @author Copyright (c) 2023 Martin Oberzalek
 */

#ifndef SRC_MEASURERESULT_H_
#define SRC_MEASURERESULT_H_

#include "dht22.h"
#include <tuple>
#include "CyclicArray.h"

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
		float dewpoint = 0;
		bool valid = false;
		WHERE where = WHERE::UNDEFINED;

		Result() = default;

		Result( WHERE where,
				DHT22::Result );

		Result( WHERE where,
				float tempCelsius,
				float tempFahrenheit,
				float humidity );
	};

protected:
	typedef std::tuple<Result,Result> RESULT_DATA;

	CyclicArray<RESULT_DATA,20> buffer;
public:

	void addMeasureResult( const Result & result );

	static float dewpoint( float temp_celsius, float humidity );

	static MeasureResult & instance();

protected:
	Result & getLastUndefined( WHERE where );
};



#endif /* SRC_MEASURERESULT_H_ */
