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

#include "tabpresenter.h"
#include "View/logscale.h"
#include "View/scaledraw.h"
#include "View/grid.h"

#include <qwt_scale_engine.h>
#include <qwt_plot_renderer.h>
#include <qlayout.h>


TabPresenter::TabPresenter (QTabWidget* parent, AbstractModel* model) :
    m_model (model), m_tab (parent), m_thread (this)
{
    if (parent)
    {
        QHBoxLayout* hl = new QHBoxLayout ();
        QVBoxLayout* vl = new QVBoxLayout ();
        vl->addLayout (hl);

        this->resize (1041, 791);
        QLabel* label = new QLabel (this);
        label->setObjectName (QStringLiteral ("label"));
        label->setGeometry (QRect (10, 10, 91, 21));
        label->setText (tr ("Row: "));
        hl->addWidget (label);

        m_source = new QComboBox (this);
        m_source->setObjectName (QStringLiteral ("cbSourceRow"));
        m_source->setGeometry (QRect (100, 10, 231, 22));
        hl->addWidget (m_source, 1);

        m_plot = new QwtPlot (this);
        m_plot->setObjectName (QStringLiteral ("qwtPlot"));
        m_plot->setGeometry (QRect (0, 40, 1041, 751));
        m_plot->setCanvasBackground (QColor (Qt::GlobalColor::white));
        m_plot->setSizePolicy (QSizePolicy (QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding));
        vl->addWidget (m_plot, 1);

        this->setLayout (vl);
    }
}


void TabPresenter::save_picture (QString filename)
{
    QwtPlotRenderer renderer;
    renderer.exportTo (m_plot, filename);
}

AbstractModel* TabPresenter::get_model() const
{
    return m_model;
}

void TabPresenter::loading_complete ()
{
    prepare_tab ();
}

void TabPresenter::set_grid ()
{
    if (!m_grid)
    {
        m_grid = std::make_unique<Grid> ();
        m_grid->attach (m_plot);
    }
    else
    {
        m_grid->detach ();
        m_grid.reset ();
    }
    m_plot->replot ();
}


void TabPresenter::set_scale_toolbar (LogScale* logScale)
{
    logScale->connect_widget (this);
}

void TabPresenter::set_scale_x (int state)
{
    if (state)
        m_plot->setAxisScaleEngine (QwtPlot::xBottom, new QwtLogScaleEngine);
    else
        m_plot->setAxisScaleEngine (QwtPlot::xBottom, new QwtLinearScaleEngine);

    m_plot->replot ();
}

void TabPresenter::set_scale_y (int state)
{
    if (state)
        m_plot->setAxisScaleEngine (QwtPlot::yLeft, new QwtLogScaleEngine);
    else
        m_plot->setAxisScaleEngine (QwtPlot::yLeft, new QwtLinearScaleEngine);

    m_plot->replot ();
}

QComboBox* TabPresenter::get_headers() const
{
    return m_source;
}

void TabPresenter::set_x_format (AxisType axis)
{
    if (axis == AxisType::TYPE_TIME)
        m_plot->setAxisScaleDraw (QwtPlot::xBottom, new YearScaleDraw ());
    else if (axis == AxisType::TYPE_NUM)
        m_plot->setAxisScaleDraw (QwtPlot::xBottom, new QwtScaleDraw ());
}

QTabWidget* TabPresenter::get_tab () const
{
    return m_tab;
}

QwtPlot* TabPresenter::get_plot() const
{
    return m_plot;
}
