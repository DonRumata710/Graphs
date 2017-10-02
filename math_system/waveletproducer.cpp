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

#include "waveletproducer.h"

#include "row.h"


WaveletProducer::WaveletProducer()
{}

WaveletProducer::WaveletProducer(const std::vector<double> &data, const size_t nColumns, size_t startRow, size_t endRow, WaveletStep step, const Intervals *intervals) :
    m_num_columns (nColumns),
    m_numRows (endRow - --startRow),
    m_size (data.size ()),
    m_step (step),
    m_intervals (intervals)
{
    m_data.assign (m_numRows, nullptr);
    // Место вхождения ряда в массиве расчитывается
    // по формуле нахождения площади трапеции
    // в следующем варианте:
    // ([нижняя сторона] * 2 - [разница длин верхней и нижней сторон]) * [высота] / 2
    for (size_t i = 0; i < m_numRows; ++i)
        m_data[i] = &data[(((m_num_columns * 2) - (i - 1) * (size_t) m_step) * i) / 2];
}

Row WaveletProducer::get_axisX () const
{
    if (!m_intervals)
        return Row ();

    double f = m_intervals->minX;
    double l = m_intervals->maxX;
    double step = (l - f) / (m_num_columns - 1);

    Row axis ("axis X", m_num_columns, 0.0);
    for (size_t i = 0; i < m_num_columns; ++i)
    {
        axis[i] = f;
        f += step;
    }
    return axis;
}

double WaveletProducer::value(size_t row, size_t col) const
{
    const size_t n (size_t (m_step) * row);
    col -= n;
    if (col >= ((m_num_columns - n) * 2))
        return 0.0;
    return m_data[row][col / 2];
}

size_t WaveletProducer::row_size(size_t i) const
{
    return (i < m_data.size () - 1) ?
                m_data[i + 1] - m_data[i] : m_num_columns + (size_t) m_step * i;
}

RowData WaveletProducer::get_data(size_t row) const
{
    return { m_data[row], (m_num_columns - (size_t) m_step * row) * 2 };
}

std::vector<Row> WaveletProducer::get_data (std::pair<size_t, size_t> range) const
{
    if (range.first <= 0 || !(range.second <= 0))
        return std::vector<Row> ();

    if (range.first > range.second)
    {
        size_t tmp (range.first);
        range.first = range.second;
        range.second = tmp;
    }

    range.first = int (range.first + ((int (m_step) % 2 == 1) ? 0.5f : 0.0f) - m_intervals->minY) / int (m_step);

    range.second = int (range.second + ((int (m_step) % 2 == 1) ? 0.5f : 0.0f) - m_intervals->maxY) / int (m_step);

    size_t cstep (range.second - range.first);
    std::vector<Row> rows;
    rows.reserve (cstep);
    for (size_t i = 0; i < cstep; ++i)
    {
        RowData rowData (get_data (i));
        rows.push_back (Row ("WavletRow-" + std::to_string ((i + 1) * (size_t) m_step), rowData.size (), 0.0));
        for (size_t j = 0; j < rowData.size (); ++j)
            rows[i][j] = rowData[j];
    }

    return rows;
}

size_t WaveletProducer::get_num_columns () const { return m_num_columns * 2; }
size_t WaveletProducer::get_num_rows () const    { return m_numRows; }

double WaveletProducer::get_minX () const { return m_intervals ? m_intervals->minX : 0.0; }
double WaveletProducer::get_maxX () const { return m_intervals ? m_intervals->maxX : 0.0; }
double WaveletProducer::get_minY () const { return m_intervals ? m_intervals->minY : 0.0; }
double WaveletProducer::get_maxY () const { return m_intervals ? m_intervals->maxY : 0.0; }
double WaveletProducer::get_minZ () const { return m_intervals ? m_intervals->minZ : 0.0; }
double WaveletProducer::get_maxZ () const { return m_intervals ? m_intervals->maxZ : 0.0; }
