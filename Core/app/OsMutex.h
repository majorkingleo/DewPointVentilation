/*
 * OsMutex.h
 *
 *  Created on: Jun 16, 2023
 *      Author: Martin Oberzalek
 */

#ifndef SRC_OSMUTEX_H_
#define SRC_OSMUTEX_H_

#include  <mutex>

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "semphr.h"

class OsMutex
{
	SemaphoreHandle_t xSemaphore = nullptr;
	StaticSemaphore_t xMutexBuffer;

public:
	OsMutex();
	~OsMutex();

	void lock();
	void unlock();
};


#endif /* SRC_OSMUTEX_H_ */
