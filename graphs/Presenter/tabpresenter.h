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
#include <QtWidgets>
#include <QtGui>
#include <qwt_plot.h>

#include "Model/abstractmodel.h"


class LogScale;
class Grid;


class TabPresenter : public QWidget
{
    Q_OBJECT

public:
    TabPresenter (QTabWidget*, AbstractModel*);

    void save (QString);

public slots:
    void set_grid ();
    void set_scale_toolbar (LogScale*);

    void set_scale_x (int);
    void set_scale_y (int);

protected:
    QComboBox* get_headers () const { return m_source; }
    AbstractModel* get_model () const { return m_model; }

protected:
    void set_x_format (AxisType);
    QTabWidget* get_tab () const;

    QwtPlot* m_plot;
    Grid* m_grid = nullptr;

private:
    AbstractModel* m_model;
    QTabWidget* m_tab;
    QComboBox* m_source;
};
