/*
 * dht22.cc
 *
 *  Created on: Apr 23, 2023
 *      Author: martin
 */
#include "dht22.h"

DHT22::DHT22( GPIO_TypeDef* gpioPort_, uint16_t gpioPin_ )
: gpioPort( gpioPort_ ),
  gpioPin( gpioPin_ )
{

}

DHT22::~DHT22()
{

}

bool DHT22::start()
{
  bool Response = false;
  GPIO_InitTypeDef GPIO_InitStructPrivate = {0};
  GPIO_InitStructPrivate.Pin = gpioPin;
  GPIO_InitStructPrivate.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructPrivate.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStructPrivate.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(gpioPort, &GPIO_InitStructPrivate);
  HAL_GPIO_WritePin (gpioPort, gpioPin, GPIO_PIN_RESET);
  microDelay (1300);
  HAL_GPIO_WritePin (gpioPort, gpioPin, GPIO_PIN_SET);
  microDelay (30);
  GPIO_InitStructPrivate.Mode = GPIO_MODE_INPUT;
  GPIO_InitStructPrivate.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(gpioPort, &GPIO_InitStructPrivate);
  microDelay (40);
  if (!(HAL_GPIO_ReadPin (gpioPort, gpioPin)))
  {
    microDelay (80);
    if ((HAL_GPIO_ReadPin (gpioPort, gpioPin))) {
    	Response = true;
    }
  }
  uint32_t pMillis = HAL_GetTick();
  uint32_t cMillis = HAL_GetTick();
  while ((HAL_GPIO_ReadPin (gpioPort, gpioPin)) && pMillis + 2 > cMillis)
  {
    cMillis = HAL_GetTick();
  }
  return Response;
}

uint8_t DHT22::read()
{
  uint8_t x,y;
  for (x=0;x<8;x++)
  {
	uint32_t pMillis = HAL_GetTick();
	uint32_t cMillis = HAL_GetTick();
    while (!(HAL_GPIO_ReadPin (gpioPort, gpioPin)) && pMillis + 2 > cMillis)
    {  // wait for the pin to go high
      cMillis = HAL_GetTick();
    }
    microDelay (40);   // wait for 40 us
    if (!(HAL_GPIO_ReadPin (gpioPort, gpioPin)))   // if the pin is low
      y&= ~(1<<(7-x));
    else
      y|= (1<<(7-x));
    pMillis = HAL_GetTick();
    cMillis = HAL_GetTick();
    while ((HAL_GPIO_ReadPin (gpioPort, gpioPin)) && pMillis + 2 > cMillis)
    {
      cMillis = HAL_GetTick();
    }
  }
  return y;
}

std::optional<DHT22::Result> DHT22::mesure()
{
	uint8_t hum1 = read();
	uint8_t hum2 = read();
	uint8_t tempC1 = read();
	uint8_t tempC2 = read();
	uint8_t SUM = read();
	uint8_t CHECK = hum1 + hum2 + tempC1 + tempC2;

	if (CHECK != SUM) {
		return {};
	}

	Result result = {};

	if (tempC1>127) {

		result.tempCelsius = (float)tempC2/10*(-1);

	} else {
		result.tempCelsius = (float)((tempC1<<8)|tempC2)/10;
	}

	result.tempFahrenheit = result.tempCelsius * 9/5 + 32;

	result.humidity = (float) ((hum1<<8)|hum2)/10;

	return result;
}

DHT22HAL::DHT22HAL( GPIO_TypeDef* gpioPort, uint16_t gpioPin, TIM_HandleTypeDef & htim_ )
: DHT22( gpioPort, gpioPin ),
  htim( htim_ )
{

}

void DHT22HAL::microDelay (uint16_t delay)
{
  __HAL_TIM_SET_COUNTER(&htim, 0);
  while (__HAL_TIM_GET_COUNTER(&htim) < delay);
}


