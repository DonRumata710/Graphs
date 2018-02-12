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


#include "maththread.h"
#include "indicator.h"
#include "math/abstractmodel.h"
#include "View/grid.h"

#include <qwt_plot.h>

#include <QtCore>
#include <QtWidgets>
#include <QtGui>

#include <memory>


class LogScale;


class TabPresenter : public QWidget
{
    Q_OBJECT

public:
    TabPresenter (QStatusBar*);

    virtual void init (QTabWidget*, AbstractModel*);

    void save_picture (QString);

    AbstractModel* get_model () const;

    void set_name (const std::string& name);
    const std::string& get_name () const;

public slots:
    void loading_complete ();

    void set_grid ();
    void set_scale_toolbar (LogScale*);

    void set_scale_x (int);
    void set_scale_y (int);

protected:
    QComboBox* get_headers () const;
    void set_x_format (AxisType);
    QTabWidget* get_tab () const;
    QwtPlot* get_plot () const;
    QStatusBar* get_status_bar () const;

protected:
    mutable MathThread m_thread;
    QComboBox* m_source = nullptr;

private:
    virtual void prepare_tab () = 0;

private:
    AbstractModel* m_model = nullptr;
    QTabWidget* m_tab = nullptr;
    QwtPlot* m_plot = nullptr;
    QStatusBar* m_status_bar = nullptr;
    std::unique_ptr<Grid> m_grid;
    std::unique_ptr<Indicator> m_indicator;
    std::string m_name;
};
