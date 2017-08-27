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


#include <vector>

#include "waveletstep.h"
#include "waveletdata.h"


struct RowData
{
public:
    RowData () {}

    RowData (const double* start, size_t size) :
        m_start (start), m_size (size)
    {}

    double operator[] (size_t i) const
    {
        return m_start[i];
    }

    size_t size () const { return m_size; }

private:
    const double* m_start;
    size_t m_size;
};



class WaveletProducer
{
public:
    WaveletProducer (const std::vector<double>& data, const size_t nColumns, size_t startRow, size_t endRow, WaveletStep step, const Intervals* intervals);

    Row get_axisX () const;

    double value (size_t row, size_t col) const;

    size_t row_size (size_t i) const;

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

    size_t m_numColumns;
    size_t m_numRows;

    const size_t m_size;

    WaveletStep m_step;

    const Intervals* m_intervals;
};


typedef std::shared_ptr<WaveletProducer> pWaveletProducer;
