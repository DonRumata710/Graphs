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

#include "matrixrasterdata.h"


MatrixRasterData::MatrixRasterData (const WaveletProducer& data) :
    data (data)
{
    setInterval (Qt::Axis::XAxis, QwtInterval (data.get_minX (), data.get_maxX ()));
    setInterval (Qt::Axis::YAxis, QwtInterval (data.get_minY (), data.get_maxY ()));
    setInterval (Qt::Axis::ZAxis, QwtInterval (data.get_minZ (), data.get_maxZ ()));
    update ();
}


void MatrixRasterData::setInterval (Qt::Axis axis, const QwtInterval &interval)
{
    QwtRasterData::setInterval (axis, interval);
    update ();
}

double MatrixRasterData::value (double x, double y) const
{
    const QwtInterval xInterval (interval (Qt::XAxis));
    const QwtInterval yInterval (interval (Qt::YAxis));

    if (!(xInterval.contains (x) && yInterval.contains (y)))
        return qQNaN ();

    size_t row ((y - yInterval.minValue ()) / dy);
    size_t col ((x - xInterval.minValue ()) / dx);
    
    if (row >= data.get_num_rows ())
        row = data.get_num_rows () - 1;
    
    return data.value (row, col);
}

std::vector<RowData> MatrixRasterData::get_data ()
{
    QwtInterval y_interval (interval (Qt::YAxis));
    size_t size ((y_interval.maxValue () - 1.4999) + 1 - (y_interval.minValue () + 1.4999));
    std::vector<RowData> vec;
    vec.reserve (size);

    for (size_t i = 0; i < size; ++i)
        vec.push_back (data.get_data (i));

    return vec;
}

void MatrixRasterData::update ()
{
    dx = 0.0;
    dy = 0.0;

    if (data.get_num_columns () > 0)
    {
        const QwtInterval& xInterval (interval (Qt::XAxis));
        if (xInterval.isValid ())
            dx = xInterval.width () / data.get_num_columns ();
        const QwtInterval& yInterval (interval (Qt::YAxis));
        if (yInterval.isValid ())
            dy = yInterval.width () / data.get_num_rows ();
    }
}
