/*
 * @author Copyright (c) 2023 Martin Oberzalek
 */
#ifndef SRC_DHT22_H_
#define SRC_DHT22_H_

#include <optional>
#include <functional>

#include "stm32f4xx_hal.h"
#include <limits>
// struct GPIO_TypeDef;

class DHT22
{
public:
	struct Result
	{
		float tempCelsius = std::numeric_limits<float>::infinity();
		float tempFahrenheit = std::numeric_limits<float>::infinity();
		float humidity = std::numeric_limits<float>::infinity();

		Result() = default;

		Result( float tempCelsius,
				float tempFahrenheit,
				float humidity )
		: tempCelsius( tempCelsius ),
		  tempFahrenheit( tempFahrenheit ),
		  humidity( humidity )
		{}
	};

protected:
	GPIO_TypeDef* gpioPort;
	const uint16_t	  gpioPin;

public:
	DHT22( GPIO_TypeDef* gpioPort, uint16_t	gpioPin );
	virtual ~DHT22();

	bool start();
	std::optional<Result> mesure();

protected:
	virtual void microDelay ( uint16_t delay ) = 0;
	uint8_t read();
};

class DHT22HAL : public DHT22
{
protected:
	TIM_HandleTypeDef & htim;

public:
	DHT22HAL( GPIO_TypeDef* gpioPort, uint16_t	gpioPin, TIM_HandleTypeDef & htim );

	void microDelay ( uint16_t delay ) override;
};

#endif /* SRC_DHT22_H_ */
