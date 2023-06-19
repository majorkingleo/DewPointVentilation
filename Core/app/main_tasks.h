/*
 * @author Copyright (c) 2023 Martin Oberzalek
 */

#ifndef INC_MAIN_TASKS_H_
#define INC_MAIN_TASKS_H_

#ifdef __cplusplus
extern "C" {
#endif

void measureInsideTask( TIM_HandleTypeDef * phtim1 );
void mesureOutsideTask( TIM_HandleTypeDef * phtim1 );
void calculateResultsTask();


#ifdef __cplusplus
}
#endif

#endif /* INC_MAIN_TASKS_H_ */
