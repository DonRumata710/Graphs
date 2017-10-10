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

#include "math/graphmodel.h"
#include "math/wavelemodel.h"

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


SpectrogramPresenter::SpectrogramPresenter (QTabWidget* parent, GraphModel* graph, const WaveletInitParams& wavelet_init_params, QStatusBar* status_bar, const std::string& name) :
    TabPresenter (status_bar),
    m_spectrogram (new QwtPlotSpectrogram (tr ("Wavlet") + " \"" + QString(wavelet_init_params.type.c_str ()) + "\" " + QString(name.c_str())))
{
    if (parent)
    {
        set_name (name + " - wavelet");
        TabPresenter::init (parent, new WaveletModel);

        m_thread.set_func([=, wavelet_init_params](){
            dynamic_cast<WaveletModel*> (get_model ())->calc_wavelet(graph, wavelet_init_params);
        });
        connect (&m_thread, SIGNAL(completed()), SLOT(loading_complete()));
        m_thread.start ();
    }
}


// Attention!!!
GraphPresenter* SpectrogramPresenter::get_local_wavlet ()
{
    SWaveletPeriod dialog;

    if (dialog.exec () == QDialog::Accepted)
    {
        GraphModel* model (new GraphModel ());
        WaveletModel* origin_model (dynamic_cast<WaveletModel*> (get_model ()));
        model->add_row (origin_model->get_data (get_headers ()->currentText ().toStdString()).get_axisX ());

        std::pair<size_t, size_t> period (dialog.get_period ());
        vector<Row> wavletLines (origin_model->get_data (get_headers ()->currentText ().toStdString()).get_data (period));

        for (Row& line : wavletLines)
            model->add_row (line);

        GraphPresenter* slice (new GraphPresenter (get_status_bar ()));
        slice->set_name (get_name () + " slice: " + std::to_string (period.first) + " / " + std::to_string (period.second));
        slice->init (qobject_cast<QTabWidget*> (parent ()), model);

        return slice;
    }
    else throw ChoiseException ();
}

void SpectrogramPresenter::attach_raster (const QString& name)
{
    WaveletModel* origin_model (dynamic_cast<WaveletModel*> (get_model ()));
    m_spectrogram->setData (new MatrixRasterData (origin_model->get_data (name.toStdString())));
    m_spectrogram->attach (get_plot ());

    const QwtInterval zInterval (m_spectrogram->data ()->interval (Qt::ZAxis));
    
    QwtScaleWidget *rightAxis = get_plot ()->axisWidget (QwtPlot::yRight);
    rightAxis->setTitle ("Intensity");
    rightAxis->setColorBarEnabled (true);
    rightAxis->setColorMap (zInterval, new ColorMap ());

    get_plot ()->setAxisScale (QwtPlot::yRight, zInterval.minValue (), zInterval.maxValue ());
    get_plot ()->enableAxis (QwtPlot::yRight);
    
    m_spectrogram->setColorMap (new ColorMap ());

    emit update_zoomer ();
    
    get_plot ()->replot ();
}

void SpectrogramPresenter::clear ()
{
    m_spectrogram->detach ();
    get_plot ()->replot ();
}

void SpectrogramPresenter::prepare_tab()
{
    connect (get_headers (), SIGNAL (currentTextChanged (const QString&)), SLOT (attach_raster (const QString&)));
    get_tab ()->addTab (this, m_spectrogram->title ().text ());

    QwtPlotCanvas *canvas = new QwtPlotCanvas (get_plot ());
    canvas->setPalette (Qt::darkGray);
    canvas->setBorderRadius (10);

    get_plot ()->setCanvas (canvas);
    get_plot ()->plotLayout ()->setAlignCanvasToScales (true);

    QwtPlotZoomer* zoomer = new Zoomer (canvas);
    connect (this, SIGNAL (update_zoomer ()), zoomer, SLOT (update_zoomer ()));

    QwtPlotPanner *panner = new QwtPlotPanner (canvas);
    panner->setMouseButton (Qt::MidButton);

    const QFontMetrics fm (get_plot ()->axisWidget (QwtPlot::yLeft)->font ());
    QwtScaleDraw* sd = get_plot ()->axisScaleDraw (QwtPlot::yLeft);
    sd->setMinimumExtent (fm.width ("100.00"));

    attach_raster (get_headers ()->itemText (0));
}
