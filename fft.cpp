/**
 * @file fft.cpp
 * @author Grupo 3 - ASSD
 * @brief
 * @version 0.1
 * @date 2023-05-05
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "fft.h"
#include "stdint.h"
#include <vector>

#define _USE_MATH_DEFINES
#include <math.h>
#define PI (float)M_PI

uint16_t bitReverse(uint16_t number, uint16_t n);

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
void fft(complex<float> *in, complex<float> *out, size_t n)
{
    // Constante para utilizar el número imaginario j
    const complex<float> j(0.0, 1.0);

    //  Se genera la tabla de exponentes “twiddle factor"
    //  W[n] = e^(-j 2 pi/N n)
    vector<complex<float>> W;

    for (uint16_t i = 0; i < n / 2; i++)
    {
        complex<float> w = exp(-j * 2.0f * PI * (float)i / (float)n);
        W.push_back(w);
    }

    //  Se inicializan las variables principales
    //  Serán uint16_t, para llegar a un tamaño máximo de 65535.

    //  GA: gamma = log2(N) = numero de etapas
    //  gr: numero de grupos por etapa
    //  mar: numero de mariposas por grupo (igual a la mitad de entradas de cada grupo)
    uint16_t gamma = (uint16_t)log2(n);
    uint16_t gr = 1;
    uint16_t mar = (uint16_t)n / 2;

    //  Se recorre cada etapa
    for (uint8_t r = 1; r < gamma + 1; r++)
    {
        //  Se recorre cada grupo dentro de la etapa
        //  gr = 2 ^(r-1)
        //  Distancia entre nodos duales (dd): dd = n / (2^r)
        //  Distancia entre grupos (dg): dg = n / (2^(r-1))

        uint16_t dd = n / (1 << r);
        uint16_t dg = n / (1 << (r-1));

        for (uint16_t g = 1; g < gr + 1; g++)
        {
            //  Se recorre cada mariposa dentro del grupo
            //  mar = n / 2^(r)
            for (uint16_t m = 1; m < mar + 1; m++)
            {
                //  Las cuentas a realizar son:
                //  Y(n) + W[k] Z(n)
                //  Y(n) - W[k] Z(n)
                //  Donde "k" va a estar definido por k = BR(2*(g-1))
                //
                //  Los indices a utilizar para la señal de entrada son entonces:
                //
                //  m + dg * (g-1) - 1
                //  m + dd + dg * (g-1) - 1
                //
                //  Donde el -1 del final es por estar utilizando "0" index (C / C++)
                //
                complex<float> R;
                complex<float> aux;

                // Si in != out, entonces en la primera etapa se utilizan los valores de in
                // En caso contrario, se utiliza el buffer out siempre, para no modificar
                // el buffer in.
                if (r == 1 && in != out)
                {
                    R = W[bitReverse(2 * (g - 1), gamma - 1)] *
                        in[m + dd + dg * (g - 1) - 1];

                    aux = in[m + dg * (g - 1) - 1];
                }
                else
                {
                    R = W[bitReverse(2 * (g - 1), gamma - 1)] *
                        out[m + dd + dg * (g - 1) - 1];

                    aux = out[m + dg * (g - 1) - 1];
                }

                //  Se reemplaza in-situ los resultados
                out[m + dg * (g - 1) - 1] = aux + R;
                out[m + dd + dg * (g - 1) - 1] = aux - R;
            }
        }
        //  Actualizacion de mar y gr
        //  Al pasar de una etapa a la siguiente, la cantidad de grupos aumenta en 2
        //  Y la cantidad de mariposas se reduce a la mitad
        mar /= 2;
        gr *= 2;
    }

    // Se devuelven los valores finales en orden bit-reverse
    // Se swapean los valores en orden
    for (uint16_t i = 0; i < n; i++)
    {
        uint16_t index = bitReverse(i, gamma);
        //  Se evita swapear 2 veces, comparando los tamaños de los indices
        if (index > i)
        {
            complex<float> aux = out[i];
            out[i] = out[index];
            out[index] = aux;
        }
    }
}

/**
 * @brief           bitReverse: calcula el bitreverse un número de n bits
 *
 * @param number    Númerero a realizar el bitRverse
 * @param n         Cantidad de bits del número
 * @return uint16_t
 */
uint16_t bitReverse(uint16_t number, uint16_t n)
{
    if (n > 16)
    {
        return 0;
    }

    //  Se copia el último bit de number a rev, y luego shiftea rev hacia la izquierda
    //  Number se shiftea hacia la derecha, para luego copiar este nuevo último bit al final de rev
    //  Así los bits se irán copiando de derecha a izquierda
    uint16_t rev = 0;
    for (uint16_t i = 0; i < n; i++)
    {
        rev <<= 1;
        rev |= number & 1;
        number >>= 1;
    }
    return rev;
}
