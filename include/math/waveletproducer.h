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

#pragma once


#include "waveletstep.h"
#include "waveletdata.h"

#include <vector>


struct RowData
{
public:
    RowData (const double* start, size_t size) :
        m_start (start), m_size (size)
    {}

    double operator[] (size_t i) const
    {
        return m_start[i];
    }

    size_t size () const { return m_size; }

    const double* get_ptr () const
    {
        return m_start;
    }

private:
    const double* m_start;
    size_t m_size;
};



class WaveletProducer
{
public:
    WaveletProducer ();
    WaveletProducer (const std::vector<double>& data, const size_t columns_num, size_t start_row, size_t end_row, WaveletStep step, const Intervals* intervals);

    Row get_axisX () const;

    double value (size_t row, size_t col) const;

    size_t get_row_size (size_t i) const;

    RowData get_data (size_t row) const;

    std::vector<Row> get_data (std::pair<size_t, size_t>) const;

    size_t get_num_columns () const;
    size_t get_num_rows () const;

    double get_minX () const;
    double get_maxX () const;
    double get_minY () const;
    double get_maxY () const;
    double get_minZ () const;
    double get_maxZ () const;

private:
    std::vector<const double*> m_data;
    size_t m_counter = 1;

    size_t m_num_columns = 0;
    size_t m_numRows = 0;

    const size_t m_size = 0;

    WaveletStep m_step = WaveletStep::ONE;

    const Intervals* m_intervals = nullptr;
};


typedef std::shared_ptr<WaveletProducer> pWaveletProducer;
