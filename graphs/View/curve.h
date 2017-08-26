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

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

#include "qwt_plot.h"
#include "qwt_plot_marker.h"
#include "qwt_plot_curve.h"

#include <memory>
using std::shared_ptr;
using std::make_shared;
using std::unique_ptr;



class Curve : public QwtPlotCurve
{
public:
    Curve (const double* x, const double* y, size_t size, QString name) : QwtPlotCurve (name)
    {
        setSamples (x, y, size);
    }

    const QString get_name () const
    {
        return title ().text ();
    }

    bool operator< (const Curve& curve) { return title ().text () < curve.title ().text (); }
    bool operator== (const Curve& curve) { return title () == curve.title (); }
};

typedef shared_ptr<Curve> pCurve;
