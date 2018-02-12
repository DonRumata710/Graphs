/////////////////////////////////////////////////////////////////////
//
// Graphs
//
// Copyright (c) 2016-2018 DonRumata710
//
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use, copy,
// modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software
// is furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
/////////////////////////////////////////////////////////////////////


#include "waveletfunction.h"

#include "threadscontrol.h"

#include <memory>
#include <limits>


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


HaarWavelet::HaarWavelet (const size_t min_points, const size_t max_points) :
    m_min_points (((min_points < max_points ? min_points : max_points) + 1) / 2),
    m_max_points (((max_points > min_points ? max_points : min_points) + 1) / 2)
{}


Intervals HaarWavelet::compute (const std::vector<double>& x, const std::vector<double>& y, std::vector<double>& wavelet, size_t& num_columns, size_t& min_points, size_t& max_points) const
{
    min_points = m_min_points;
    max_points = (m_max_points * 2 > x.size ()) ? x.size () / 2 : m_max_points;
    const size_t num_rows (max_points + 1 - m_min_points);
    num_columns = x.size () - 1 - (m_min_points - 1) * 2;

    const size_t num_threads (prepare_threads ());

    wavelet.assign ((num_columns + 1 - num_rows) * num_rows, 0.0);

    std::unique_ptr<double> minZ (new double[omp_get_max_threads ()]);
    std::unique_ptr<double> maxZ (new double[omp_get_max_threads ()]);

  #pragma omp parallel
    {
        int threadNum (omp_get_thread_num ());
        minZ.get ()[threadNum] = std::numeric_limits<double>::max ();
        maxZ.get ()[threadNum] = std::numeric_limits<double>::lowest ();
    }

  #pragma omp parallel for schedule(dynamic, 16)
    for (int i = 0; i < num_rows; ++i)
    {    // Перебор всех рядов
        const size_t branch (i + m_min_points);          // Расчет длины каждой части функции
        const size_t rowSize (num_columns - i * 2);      // Расчет количества значений в ряду
        const size_t rowOffset ((num_columns - (i - 1)) * i);  // Сдвиг адресации ряда в векторе
        for (int j = 0; j < rowSize; ++j)
        {    // Перебор всех точек
            const size_t offset (rowOffset + j);
            size_t marker (branch);
            for (size_t k = 0; k < marker; ++k)
                wavelet[offset] -= y[j + k];
            marker += branch;
            for (size_t k = marker - branch; k < marker; ++k)
                wavelet[offset] += y[j + k];
            wavelet[offset] /= marker;

            if (minZ.get ()[omp_get_thread_num ()] > wavelet[offset])
                minZ.get ()[omp_get_thread_num ()] = wavelet[offset];

            if (maxZ.get ()[omp_get_thread_num ()] < wavelet[offset])
                maxZ.get ()[omp_get_thread_num ()] = wavelet[offset];
        }
    }

    for (int i = 1; i < num_threads; ++i)
    {
        if (minZ.get ()[0] > minZ.get ()[i])
            minZ.get ()[0] = minZ.get ()[i];
        if (maxZ.get ()[0] < maxZ.get ()[i])
            maxZ.get ()[0] = maxZ.get ()[i];
    }

    return { x[0], x[x.size () - 1], double (m_min_points * 2 - 1), double (max_points * 2 + 1), minZ.get()[0], maxZ.get()[0] };
}



FhatWavelet::FhatWavelet (const size_t min_points, const size_t max_points) :
    m_min_points (((min_points < max_points ? min_points : max_points) + 2) / 3),
    m_max_points (((max_points > min_points ? max_points : min_points) + 2) / 3)
{}

Intervals FhatWavelet::compute (const std::vector<double>& x, const std::vector<double>& y, std::vector<double>& wavelet, size_t& num_columns, size_t& min_points, size_t& max_points) const
{
    min_points = m_min_points;
    max_points = (m_max_points * 3 > x.size ()) ? x.size () / 3 : m_max_points;
    size_t num_rows (max_points + 1 - m_min_points);
    size_t num_threads (prepare_threads ());
    num_columns = x.size () - 2 - (m_min_points - 1) * 3;

    wavelet.assign ((((num_columns * 2) - (num_rows - 1) * 3) * num_rows) / 2, 0.0);

    std::unique_ptr<double> minZ (new double[omp_get_max_threads ()]);
    std::unique_ptr<double> maxZ (new double[omp_get_max_threads ()]);

  #pragma omp parallel
    {
        int threadNum (omp_get_thread_num ());
        minZ.get ()[threadNum] = std::numeric_limits<double>::max ();
        maxZ.get ()[threadNum] = std::numeric_limits<double>::lowest ();
    }

  #pragma omp parallel for schedule(dynamic, 16)
    for (int i = 0; i < num_rows; ++i)
    {    // Перебор всех рядов
        const size_t branch (i + m_min_points);          // Расчет длины каждой части функции
        const size_t rowSize (num_columns - i * 3);      // Расчет количества значений в ряду
        const size_t rowOffset ((((num_columns * 2) - (i - 1) * 3) * i) / 2);         // Сдвиг адресации ряда в векторе
        for (size_t j = 0; j < rowSize; ++j)
        {    // Перебор всех точек
            const size_t offset (rowOffset + j);
            size_t marker (branch);
            for (size_t k = 0; k < marker; ++k)
                wavelet[offset] -= y[j + k] / 2.0;
            marker += branch;
            for (size_t k = marker - branch; k < marker; ++k)
                wavelet[offset] += y[j + k];
            marker += branch;
            for (size_t k = marker - branch; k < marker; ++k)
                wavelet[offset] -= y[j + k] / 2.0;
            wavelet[offset] /= marker;

            if (minZ.get ()[omp_get_thread_num ()] > wavelet[offset])
                minZ.get ()[omp_get_thread_num ()] = wavelet[offset];

            if (maxZ.get ()[omp_get_thread_num ()] < wavelet[offset])
                maxZ.get ()[omp_get_thread_num ()] = wavelet[offset];
        }
    }

    for (int i = 1; i < num_threads; ++i)
    {
        if (minZ.get ()[0] > minZ.get ()[i])
            minZ.get ()[0] = minZ.get ()[i];
        if (maxZ.get ()[0] < maxZ.get ()[i])
            maxZ.get ()[0] = maxZ.get ()[i];
    }

    return { x[0], x[x.size () - 1], m_min_points * 3 - 1.5, max_points * 3 + 1.5, minZ.get ()[0], maxZ.get ()[0] };
}



MorletWavelet::MorletWavelet (const size_t min_points, const size_t max_points, const size_t num_periods) :
    m_num_periods (num_periods),
    m_max_points (max_points > min_points ? max_points : min_points),
    m_min_points (max_points < min_points ? max_points : min_points)
{}


Intervals MorletWavelet::compute (const std::vector<double>& x, const std::vector<double>& y, std::vector<double>& wavelet, size_t& num_columns, size_t& min_points, size_t& max_points) const
{
    const size_t num_threads (prepare_threads ());

    min_points = (m_min_points < m_num_periods * 2) ? m_min_points : m_num_periods * 2;
    max_points = y.size () < m_max_points ? y.size () : m_max_points;
    num_columns = y.size () + 1 - min_points;     // количество чисел в нижнем ряду
    const size_t num_rows (max_points + 1 - min_points);

    wavelet.assign ((num_columns + y.size () + 1 - max_points) * num_rows / 2, 0.0);

    std::unique_ptr<double> minZ (new double[num_threads]);
    std::unique_ptr<double> maxZ (new double[num_threads]);

  #pragma omp parallel
    {
        int thread_num (omp_get_thread_num ());
        minZ.get ()[thread_num] = std::numeric_limits<double>::max ();
        maxZ.get ()[thread_num] = std::numeric_limits<double>::lowest ();
    }

  #pragma omp parallel for schedule(dynamic, 16)
    for (int i = 0; i < num_rows; ++i)
    {    // Перебор всех рядов
        std::vector<std::complex<double>> morlet_factors (m_calc_morlet_factors (i + min_points));
        std::vector<std::complex<double>> row (y.size () + 1 - morlet_factors.size ());
        for (size_t j = 0; j < row.size (); ++j)
        {
            for (size_t k = 0; k < morlet_factors.size (); ++k)
                row[j] += y[j + k] * morlet_factors[k];

            double index ((num_columns * 2 + 1 - i) * i / 2 + j);
            wavelet[index] =
          #if 0
                sqrt (collection[j].real () * collection[j].real () + collection[j].imag () * collection[j].imag ()) /
                ((morlet_factors.size ()) * (morlet_factors.size ()));
          #else
                row[j].imag ();
          #endif

            if (minZ.get ()[omp_get_thread_num ()] > wavelet[index])
                minZ.get ()[omp_get_thread_num ()] = wavelet[index];

            if (maxZ.get ()[omp_get_thread_num ()] < wavelet[index])
                maxZ.get ()[omp_get_thread_num ()] = wavelet[index];
        }
    }

    for (int i = 1; i < num_threads; ++i)
    {
        if (minZ.get ()[0] > minZ.get ()[i])
            minZ.get ()[0] = minZ.get ()[i];
        else
        if (maxZ.get ()[0] < maxZ.get ()[i])
            maxZ.get ()[0] = maxZ.get ()[i];
    }

    return { x[0], x[x.size () - 1], double (min_points), double (max_points), minZ.get ()[0], maxZ.get ()[0] };
}


std::vector<std::complex<double>> MorletWavelet::m_calc_morlet_factors(const int num_points) const
{
    std::vector<std::complex<double>> data (num_points);
    const int real_points (num_points * 2 + 1);
    for (int i = 0; i < num_points; ++i)
    {
        double x (((i * 2) + 1 - real_points / 2.0) * m_num_periods / double (real_points));
        std::complex<double> val (cos (2 * M_PI * x), sin (2 * M_PI * x));
        double haus (exp (-2 * x * x * M_PI * M_PI / (m_num_periods * m_num_periods)));
        val = val * haus;
        data[i] = val;
    }
    return data;
}
