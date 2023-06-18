/*
 * @author Copyright (c) 2023 Martin Oberzalek
 */

#include "OsMutex.h"

OsMutex::OsMutex()
{
	 xSemaphore = xSemaphoreCreateMutexStatic( &xMutexBuffer );
	 /* The pxMutexBuffer was not NULL, so it is expected that the
	     handle will not be NULL. */
	 configASSERT( xSemaphore );
}

OsMutex::~OsMutex()
{
	vSemaphoreDelete( xSemaphore );
}

void OsMutex::lock()
{
	if( xSemaphoreTake( xSemaphore, ( TickType_t ) portMAX_DELAY ) != pdTRUE ) {
		printf( "mutex locking failed\r\n");
		return;
	}
}

void OsMutex::unlock()
{
	if( xSemaphoreGive( xSemaphore ) != pdTRUE ) {
		printf( "mutex unlocking failed\r\n");
		return;
	}
}


