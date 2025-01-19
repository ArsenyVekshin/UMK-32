#pragma once

int ResultIP[] = {0, 0, 0, 0};

void StringToIP(String Data)
{
  Data = Data + '.';
  int L = Data.length();

  int Car = Data.indexOf('=');
  int preCar = Data.indexOf('=');

  for (int i = 0;i <= 3;i++)
  {
  Car = Car + Data.substring(Car+1, L).indexOf('.') + 1;
  ResultIP[i] = Data.substring(preCar+1, Car).toInt();
  preCar = Car;
  }
}

void(*reset)(void) = 0;
