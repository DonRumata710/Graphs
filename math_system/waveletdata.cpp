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

#include "waveletdata.h"
#include "row.h"
#include "waveletproducer.h"

#include "threadscontrol.h"

#include <atomic>

#include <iostream>

#include <math.h>
#include <complex>


WaveletData::WaveletData (const std::vector<double>& x, const std::vector<double>& y, const WaveletFunction* function)
{
    columns = x.size ();

    size_t num_columns (0);
    size_t min_points (0);
    size_t max_points (0);
    m_intervals = function->compute (x, y, wavelet, num_columns, min_points, max_points);

    data.reset (new WaveletProducer (wavelet, num_columns, min_points, max_points, function->get_step (), &m_intervals));
}

const WaveletProducer& WaveletData::get_producer () const
{
    return *data;
}


void WaveletData::save_data (pDocument document)
{
    /*
    if (wavelet.empty ()) return;

    QAxObject* Cell1 (sheet->querySubObject ("Cells(QVariant&,QVariant&)", QVariant (1), QVariant (1)));
    QAxObject* Cell2 (sheet->querySubObject ("Cells(QVariant&,QVariant&)", QVariant (columns + 1), QVariant (wavelet.size () / columns + 1)));
    QAxObject* range (sheet->querySubObject ("Range(const QVariant&,const QVariant&)", Cell1->asVariant (), Cell2->asVariant ()));

    QList<QVariant> cellsList;
    QList<QVariant> rowsList;

    cellsList << QString::number (0);
    for (int i = int (m_intervals.minY + 1.50001); i < int (m_intervals.maxY - 1.49999); i += (m_isFrenchHat ? 3 : 2))
        cellsList << QString::number (i);
    rowsList << QVariant (cellsList);

    double step ((m_intervals.maxX - m_intervals.minX) / columns);

    for (int j = 0; j < columns; j++)
    {
        cellsList.clear ();
        cellsList << step * j;
        for (int i = 0; i < wavelet.size () / columns; i++)
            cellsList << wavelet[i * columns + j];
        rowsList << QVariant (cellsList);
    }

    range->setProperty ("Value", QVariant (rowsList));
    */
}
