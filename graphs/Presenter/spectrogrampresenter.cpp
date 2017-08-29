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

#include "spectrogrampresenter.h"
#include "graphpresenter.h"
#include "matrixrasterdata.h"

#include "View/logscale.h"
#include "View/scaledraw.h"
#include "View/zoomer.h"
#include "View/swaveletperiod.h"
#include "View/choiseexception.h"

#include "Model/graphmodel.h"
#include "Model/wavelemodel.h"

#include <qwt_plot_panner.h>
#include <qwt_plot.h>
#include <qwt_scale_engine.h>

#include <qwt_color_map.h>
#include <qwt_plot_layout.h>
#include <qwt_matrix_raster_data.h>
#include <qwt_scale_widget.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_renderer.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_canvas.h>


class WaveletFunction;


class ColorMap : public QwtLinearColorMap
{
public:
    ColorMap () : QwtLinearColorMap (Qt::GlobalColor::darkBlue, Qt::GlobalColor::white)
    {
        addColorStop (0.8, Qt::GlobalColor::red);
        addColorStop (0.6, Qt::GlobalColor::yellow);
        //addColorStop (0.4, Qt::GlobalColor::cyan);
        addColorStop (0.4, Qt::GlobalColor::blue);
    }
};


SpectrogramPresenter::SpectrogramPresenter (QTabWidget* parent, GraphModel* graph, const WaveletInitParams& wavelet_init_params) :
    TabPresenter (parent, new WaveletModel (graph, func)),
    m_spectrogram (new QwtPlotSpectrogram (tr ("Wavlet") + " \"" + QString(wavelet_init_params.type) + "\" " + QString(graph->get_name ().c_str())))
{
    if (parent)
    {
        connect (get_headers (), SIGNAL (currentTextChanged (const QString&)), SLOT (attach_raster (const QString&)));
        parent->addTab (this, tr ("Wavelet") + " \"" + QString(wavelet_init_params.type) + "\" "+ QString(graph->get_name ().c_str ()));

        QwtPlotCanvas *canvas = new QwtPlotCanvas (m_plot);
        canvas->setPalette (Qt::darkGray);
        canvas->setBorderRadius (10);

        m_plot->setCanvas (canvas);
        m_plot->plotLayout ()->setAlignCanvasToScales (true);

        QwtPlotZoomer* zoomer = new Zoomer (canvas);
        connect (this, SIGNAL (update_zoomer ()), zoomer, SLOT (update_zoomer ()));

        QwtPlotPanner *panner = new QwtPlotPanner (canvas);
        panner->setMouseButton (Qt::MidButton);

        const QFontMetrics fm (m_plot->axisWidget (QwtPlot::yLeft)->font ());
        QwtScaleDraw* sd = m_plot->axisScaleDraw (QwtPlot::yLeft);
        sd->setMinimumExtent (fm.width ("100.00"));

        attach_raster (get_headers ()->itemText (0));
    }
}


// Attention!!!
GraphPresenter* SpectrogramPresenter::get_local_wavlet ()
{
    SWaveletPeriod dialog;

    if (dialog.exec () == QDialog::Accepted)
    {
        GraphModel* model (new GraphModel ());
        model->add_row (get_model ()->get_data (get_headers ()->currentText ().toStdString()).get_axisX ());

        vector<Row> wavletLines (get_model ()->get_data (get_headers ()->currentText ().toStdString()).get_data (dialog.get_period ()));

        for (Row& line : wavletLines) model->add_row (line);

        return new GraphPresenter (qobject_cast<QTabWidget*> (parent ()), model);
    }
    else throw ChoiseException ();
}


void SpectrogramPresenter::attach_raster (const QString& name)
{
    m_spectrogram->setData (new MatrixRasterData (get_model ()->get_data (name.toStdString())));
    m_spectrogram->attach (m_plot);

    const QwtInterval zInterval (m_spectrogram->data ()->interval (Qt::ZAxis));
    
    QwtScaleWidget *rightAxis = m_plot->axisWidget (QwtPlot::yRight);
    rightAxis->setTitle ("Intensity");
    rightAxis->setColorBarEnabled (true);
    rightAxis->setColorMap (zInterval, new ColorMap ());

    m_plot->setAxisScale (QwtPlot::yRight, zInterval.minValue (), zInterval.maxValue ());
    m_plot->enableAxis (QwtPlot::yRight);
    
    m_spectrogram->setColorMap (new ColorMap ());

    emit update_zoomer ();
    
    m_plot->replot ();
}

void SpectrogramPresenter::clear ()
{
    m_spectrogram->detach ();
    m_plot->replot ();
}

WaveletModel* SpectrogramPresenter::get_model ()
{
    return (WaveletModel*) TabPresenter::get_model ();
}
