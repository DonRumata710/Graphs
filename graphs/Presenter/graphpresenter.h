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

#include "View/curve.h"
#include "tabpresenter.h"

#include <vector>
#include <memory>

using std::vector;
typedef std::shared_ptr<QPolygonF> pPolygon;


class MultyGraphTools;
class Row;
class GraphModel;
class SpectrogramPresenter;
class QAxObject;
class PlotData;


class GraphPresenter : public TabPresenter
{
    Q_OBJECT

public:
    GraphPresenter (QTabWidget* parent, QAxObject*);
    GraphPresenter (QTabWidget* parent, GraphModel*);

    void add_multy_graph_tools (MultyGraphTools*);

    GraphModel* get_model () const;

    GraphPresenter* get_deviations () const;
    GraphPresenter* get_smoothing () const;
    GraphPresenter* get_spectr () const;
    GraphPresenter* get_correlations () const;

    GraphPresenter* get_power () const;

    SpectrogramPresenter* get_wavelet () const;

public slots:
    void set_current_curve (const QString&);
    void attach_curve ();
    void detach_curve ();

    void attach_curve (const QString&);
    void detach_curve (const QString&);
    void clear ();

private:
    void add_curve (const vector<double>& axis, const vector<double>& source, const QString);

    vector<pCurve> m_curves;
    QString m_current;

    size_t m_count;
};
