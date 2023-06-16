/*
 * MeasureResult.h
 *
 *  Created on: Jun 16, 2023
 *      Author: Martin Oberzalek
 */

#ifndef SRC_MEASURERESULT_H_
#define SRC_MEASURERESULT_H_

#include "dht22.h"

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

		Result( WHERE where,
				DHT22::Result );

		Result( WHERE where,
				float tempCelsius,
				float tempFahrenheit,
				float humidity );
	};

	static float dewpoint( float temp_celsius, float humidity );
};



#endif /* SRC_MEASURERESULT_H_ */
