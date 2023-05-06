/**
 * @file main.cpp
 * @author Grupo 3 - ASSD
 * @brief
 * @version 0.1
 * @date 2023-05-06
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <iostream>
#include "fft.h"

#define _USE_MATH_DEFINES
#include <math.h>
#define PI (float)M_PI

using namespace std;

void cosineSignal(float amp, float freq, float phase, complex<float> *x, size_t n);
void expSignal(float amp, float freq, float phase, complex<float> *x, size_t n);

int main(int argc, char **argv)
{
    size_t n = 8;
    complex<float> *in = new complex<float>[n];
    complex<float> *out = in;

    // cosineSignal(1.0f, 1.0f / 8, 0.0f, in, n);
    expSignal(1.0f, 1.0f / 4, 0.0f, in, n);

    fft(in, out, n);

    // Se imprimen los valores resultantes
    for (int i = 0; i < n; i++)
    {
        cout << out[i].real() << " + j " << out[i].imag() << endl; 
    }

    delete[] in;

    return 0;
}

/**
 * @brief           Devuelve un vector con un coseno discreto de frecuencia, amplitud y fase
 *                  indicadas.
 *
 * @param amp       Amplitud
 * @param freq      Frecuencia
 * @param phase     Fase
 * @param x         Vector para almacenar los valores
 * @param n         Tamaño del vector
 */
void cosineSignal(float amp, float freq, float phase, complex<float> *x, size_t n)
{
    complex<float> j(0, 1);
    for (int i = 0; i < n; i++)
    {
        x[i] = (amp * exp(j * 2.0f * PI * freq * (float)i + phase) +
                amp * exp(-(j * 2.0f * PI * freq * (float)i + phase))) /
               2.0f;
    }
}

/**
 * @brief           Devuelve un vector con una exponencial compleja discreto de frecuencia,
 *                  amplitud y fase indicadas.
 *
 * @param amp       Amplitud
 * @param freq      Frecuencia
 * @param phase     Fase
 * @param x         Vector para almacenar los valores
 * @param n         Tamaño del vector
 */
void expSignal(float amp, float freq, float phase, complex<float> *x, size_t n)
{
    complex<float> j(0, 1);
    for (int i = 0; i < n; i++)
    {
        x[i] = amp * exp(j * 2.0f * PI * freq * (float)i + phase);
    }
}