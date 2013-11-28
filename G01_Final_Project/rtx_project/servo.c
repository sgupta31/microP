#include "servo.h"
#include "pwm.h"

extern float alpha;
extern float beta;
float angle_delta = (1.0);

void alpha_motor(float alpha){
	
		if ((alpha < 180) && (alpha >=0)) {
			TIM3->CCR1 = 1100 + (alpha * (400.0/45.0));
		}
		
}

void beta_motor(float beta){
	
		if ((beta < 180) && (beta >=0)) {
			TIM3->CCR2 = 1100 + (beta * (400.0/45.0));
		}
// 		if (beta <= 180) {
// 			beta += angle_delta;
// 		}

}
