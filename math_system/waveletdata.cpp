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

#include "waveletdata.h"
#include "row.h"
#include "waveletproducer.h"
#include "waveletfunction.h"
#include "threadscontrol.h"

#include <atomic>
#include <iostream>

#include <math.h>
#include <complex>


WaveletData::WaveletData (const std::vector<double>& x, const std::vector<double>& y, const WaveletInitParams& wavelet_init_params)
{
    columns = x.size ();

    std::unique_ptr<WaveletFunction> function;
    if (wavelet_init_params.type == "HAAR")
    {
        function = std::make_unique<HaarWavelet>(wavelet_init_params.min_points_num, wavelet_init_params.max_points_num);
    }
    else if (wavelet_init_params.type == "FHAT")
    {
        function = std::make_unique<FhatWavelet>(wavelet_init_params.min_points_num, wavelet_init_params.max_points_num);
    }
    else if (wavelet_init_params.type == "MORLET")
    {
        function = std::make_unique<MorletWavelet>(wavelet_init_params.min_points_num,
                                                   wavelet_init_params.max_points_num,
                                                   wavelet_init_params.morlet_length);
    }

    size_t num_columns (0);
    size_t min_points (0);
    size_t max_points (0);
    m_intervals = function->compute (x, y, m_wavelet, num_columns, min_points, max_points);

    data.reset (new WaveletProducer (m_wavelet, num_columns, min_points, max_points, function->get_step (), &m_intervals));

    m_step = (size_t) function->get_step();
}

const WaveletProducer& WaveletData::get_producer () const
{
    return *data;
}

void WaveletData::save_data (pPage page)
{
    if (m_wavelet.empty ())
        return;

    std::vector<double> axis;
    double step ((m_intervals.maxX - m_intervals.minX) / data->get_num_columns ());
    for (double i = m_intervals.minX + step / 2.0; i < m_intervals.maxX; i += step)
        axis.push_back (i);
    page->push_data_back ("x axis", axis);

    for (int i = 0; i < data->get_num_rows (); i++)
    {
        RowData row (data->get_data (i));
        page->push_data_back (std::to_string (row.size ()), row.get_ptr (), row.size ());
    }
}
