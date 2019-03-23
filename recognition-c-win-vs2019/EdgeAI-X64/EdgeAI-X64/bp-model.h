#pragma once
#ifndef __MODEL__H
#define __MODEL__H

#include "sys.h"
void initBPNework();
void computO(int var);
void backUpdate(int var);
void  trainNetwork();
double result(double var1, double var2);

#endif // !__MODEL__H
