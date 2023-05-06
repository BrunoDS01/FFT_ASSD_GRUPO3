/**
 * @file fft.h
 * @author Grupo 3 - ASSD
 * @brief 
 * @version 0.1
 * @date 2023-05-05
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef FFT
#define FFT

#include <complex>
using namespace std;

/**
 * @brief       FFT: Realiza la Fast Fourier Transform de una señal digital
 *              mediante el algoritmo de Cooley-Tukey
 * 
 * @param in    Puntero al buffer con datos de entrada.
 * @param out   Puntero al buffer con datos de salida.
 * @param n     Cantidad de puntos. En caso de no ser una potencia de dos, el comportamiento
 *              es indeterminado.
 * 
 *              El buffer de salida puede ser el mismo que el buffer de entrada.
 */
void fft(complex <float> * in, complex <float> *out, size_t n);

#endif // FFT