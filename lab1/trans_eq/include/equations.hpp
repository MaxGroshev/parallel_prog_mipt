#pragma once

#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>

#include <mpi.h>

//-----------------------------------------------------------------------------

namespace types_of_eq {
    class transf_eq;
}

template <typename T>
class equation_t {
    public:
        equation_t() = delete;

        const double t_max = 50.0; // Максимальное время
        const double x_max = 60.0; // Максимальная координата
        const double t_step = 0.1;       // Шаг по времени
        const double x_step = 0.1; // Шаг по координате
        const double alpha = 1.0;  // Коэффициент переноса
        // Функция правой части уравнения
        inline double func(double t, double x) const {}
        // Начальное условие
        inline double fi(double x) const {}
        // Граничное условие
        inline double ksi(double t) const {}
};

template <>
class equation_t<types_of_eq::transf_eq> {
    public:
        const double t_max = 2.0; 
        const double x_max = 2.0; 
        const double t_step = 0.01;       
        const double x_step = 0.01; 
        const double alpha = 1.0;  

        inline double func(double t, double x) const { return x * t; }        
        inline double fi(double x) const { return x * x / 3; }
        inline double ksi(double t) const { return -t * t; }
};


