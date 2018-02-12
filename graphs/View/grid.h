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

#include <qwt_plot_grid.h>


class Grid : public QwtPlotGrid
{
public:
    Grid ()
    {
        enableXMin (true);
        setMajorPen (Qt::black, 0, Qt::DotLine);
        setMinorPen (Qt::gray, 0, Qt::DotLine);
    }

    virtual void updateScaleDiv (const QwtScaleDiv &xScaleDiv,
        const QwtScaleDiv &yScaleDiv)
    {
        QwtScaleDiv scaleDiv (xScaleDiv.lowerBound (),
            xScaleDiv.upperBound ());

        scaleDiv.setTicks (QwtScaleDiv::MinorTick,
            xScaleDiv.ticks (QwtScaleDiv::MinorTick));
        scaleDiv.setTicks (QwtScaleDiv::MajorTick,
            xScaleDiv.ticks (QwtScaleDiv::MajorTick));

        QwtPlotGrid::updateScaleDiv (scaleDiv, yScaleDiv);
    }
};
