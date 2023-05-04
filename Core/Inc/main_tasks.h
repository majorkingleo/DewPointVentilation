/*
 * main_tasks.h
 *
 *  Created on: 26.04.2023
 *      Author: martin
 */

#ifndef INC_MAIN_TASKS_H_
#define INC_MAIN_TASKS_H_

#ifdef __cplusplus
extern "C" {
#endif

void main_task( TIM_HandleTypeDef * phtim1 );
void mesureOutsideTask( TIM_HandleTypeDef * phtim1 );


#ifdef __cplusplus
}
#endif

#endif /* INC_MAIN_TASKS_H_ */
