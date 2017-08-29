/////////////////////////////////////////////////////////////////////
//
// Graphs
//
// Copyright (c) 2016-2017 DonRumata710
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


const double M_PI = 3.14159265358979323846;


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

    wavelet.assign ((num_columns - (max_points - 1)) * num_rows, 0.0);

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
    {    // ������� ���� �����
        const size_t branch (i + m_min_points);          // ������ ����� ������ ����� �������
        const size_t rowSize (num_columns - i * 2);      // ������ ���������� �������� � ����
        const size_t rowOffset ((num_columns - (i - 1)) * i);  // ����� ��������� ���� � �������
        for (int j = 0; j < rowSize; ++j)
        {    // ������� ���� �����
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
            else
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

    wavelet.assign ((((num_columns * 2) - (max_points - 1) * 3) * num_rows) / 2, 0.0);

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
    {    // ������� ���� �����
        const size_t branch (i + m_min_points);          // ������ ����� ������ ����� �������
        const size_t rowSize (num_columns - i * 3);      // ������ ���������� �������� � ����
        const size_t rowOffset ((((num_columns * 2) - (i - 1) * 3) * i) / 2);         // ����� ��������� ���� � �������
        for (size_t j = 0; j < rowSize; ++j)
        {    // ������� ���� �����
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
            else
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



MorletWavelet::MorletWavelet (size_t num_periods) :
    m_num_periods (num_periods),
    m_min_points (num_periods * 2)
{}


Intervals MorletWavelet::compute (const std::vector<double>& x, const std::vector<double>& y, std::vector<double>& wavelet, size_t& num_columns, size_t& min_points, size_t& max_points) const
{
    const size_t m_num_threads (prepare_threads ());

    min_points = m_min_points;
    max_points = y.size ();
    num_columns = y.size () + 1 - m_min_points;
    const size_t num_rows (num_columns);

    wavelet.assign ((num_columns + 1) * num_rows / 2, 0.0);

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
    {    // ������� ���� �����
        std::vector<std::complex<double>> morlet_factors;
        m_calc_morlet_factors (i + m_min_points, morlet_factors);
        size_t num_indexes (y.size () + 1 - morlet_factors.size ());
        for (size_t j = 0; j < num_indexes; ++j)
        {
            std::vector<std::complex<double>> collection (num_indexes);
            for (size_t k = 0; k < morlet_factors.size (); ++k)
                collection[j] += y[j + k] * morlet_factors[k];

            double index ((i * num_rows) / 2 + j);
            wavelet[index] =
          #if 1
                sqrt (collection[j].real () * collection[j].real () + collection[j].imag () * collection[j].imag ()) /
                ((morlet_factors.size ()) * (morlet_factors.size ()));
          #else
                collection[j].imag () / morlet_factors.size ();
          #endif

            if (minZ.get ()[omp_get_thread_num ()] > wavelet[index])
                minZ.get ()[omp_get_thread_num ()] = wavelet[index];

            if (maxZ.get ()[omp_get_thread_num ()] < wavelet[index])
                maxZ.get ()[omp_get_thread_num ()] = wavelet[index];
        }
    }

    for (int i = 1; i < m_num_threads; ++i)
    {
        if (minZ.get ()[0] > minZ.get ()[i])
            minZ.get ()[0] = minZ.get ()[i];
        else
        if (maxZ.get ()[0] < maxZ.get ()[i])
            maxZ.get ()[0] = maxZ.get ()[i];
    }

    return { x[0], x[x.size () - 1], double (m_num_periods / 2), double (num_rows + m_num_periods), minZ.get ()[0], maxZ.get ()[0] };
}


void MorletWavelet::m_calc_morlet_factors (const int num_points, std::vector<std::complex<double>>& data) const
{
    data.assign (num_points, 0.0);
    const int real_points (num_points * 2 + 1);
    for (int i = 0; i < num_points; ++i)
    {
        double x (((i * 2) + 1 - real_points / 2.0) * m_num_periods / double (real_points));
        std::complex<double> val (cos (2 * M_PI * x), sin (2 * M_PI * x));
        double haus (exp (-2 * x * x * M_PI * M_PI / (m_num_periods * m_num_periods)));
        val = val * haus;
        data[i] = val;
    }
}