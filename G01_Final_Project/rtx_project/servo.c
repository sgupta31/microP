#include "servo.h"

float alpha = 0.0;
float beta = 0.0;
float angle_delta = (1.0);

void alpha_motor(float alpha){
	
		TIM3->CCR2 = 1050 + (alpha * (450.0/45.0));
		
		alpha += angle_delta;
		
		if ((alpha >= 90) || (alpha <= -90)) {
			angle_delta = -angle_delta;
			alpha += angle_delta;
		}
}

void beta_motor(float beta){
		
		TIM3->CCR2 = 1050 + (alpha * (450.0/45.0));
		
		beta += angle_delta;
		
		if ((beta >= 90) || (beta <= -90)) {
			angle_delta = -angle_delta;
			beta += angle_delta;
		}
}
