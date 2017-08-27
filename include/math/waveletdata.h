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

#pragma once


#include "document/document.h"

#include <vector>
#include <memory>
#include <complex>

#include "waveletfunction.h"


class Row;
class WaveletProducer;


class WaveletData
{
public:
    WaveletData (const std::vector<double>& x, const std::vector<double>& y, const WaveletFunction* function);

    const WaveletProducer& get_producer () const;

    void save_data (pDocument document);

private:
    /*
     * void m_haaf_wavelet (const vector<double>&, const vector<double>&, vector<double>&, size_t, size_t);
     * void m_french_hat_wavelet (const vector<double>&, const vector<double>&, vector<double>&, size_t, size_t);
     * void m_morlet_wavelet (const vector<double>&, const vector<double>&, vector<double>&, size_t, size_t);
     * void m_calc_morlet_factors (const int num_points, const int min_points, std::vector<std::complex<double>>& data);
     */

    std::vector<double> wavelet;
    size_t columns = 0;

    Intervals m_intervals;

    std::unique_ptr<WaveletProducer> data;

    bool m_isFrenchHat = false;

    static size_t m_numThreads;
};
