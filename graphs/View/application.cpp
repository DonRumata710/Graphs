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


#include "application.h"

#include "spectr.h"
#include "correlation.h"
#include "wavelet.h"

#include "helper.h"

#include "logscale.h"
#include "multygraph.h"

#include <qfiledialog.h>

#include <memory>
using std::unique_ptr;
using std::shared_ptr;


Application::Application(QWidget *parent) : QMainWindow (parent)
{
    m_grid.setupUi (this);

    setCentralWidget (m_grid.tabWidget);
    m_manager.set_widgets (m_grid.tabWidget, m_grid.statusBar);

    m_grid.aLogScale->setCheckable (true);
    m_grid.aMultyGraph->setCheckable (true);

    connect (m_grid.open,   SIGNAL (triggered ()), SLOT (open ()));
    connect (m_grid.save,   SIGNAL (triggered ()), SLOT (save ()));
    connect (m_grid.saveas, SIGNAL (triggered ()), SLOT (save_as ()));
    connect (m_grid.close,  SIGNAL (triggered ()), SLOT (close ()));
    connect (m_grid.about,  SIGNAL (triggered ()), SLOT (help ()));
    connect (m_grid.exit,   SIGNAL (triggered ()), qApp, SLOT (quit ()));

    connect (m_grid.tabWidget, SIGNAL (tabCloseRequested (int)), &m_manager, SLOT (close_graph (int)));
}

void Application::set_filename (const QString filename)
{
    m_filename = filename;
}



void Application::open ()
{
    QString filename = QFileDialog::getOpenFileName (this, tr ("Load"), ".", tr ("Excel files (*.xls);;CSV files (*.csv)"));

    if (!filename.isEmpty ())
        open_file (filename);
}

void Application::save ()
{
    save_doc (m_filename);
}

void Application::save_as ()
{
    QString filename = QFileDialog::getSaveFileName (this, tr ("Save as"), ".", tr ("Excel files (*.xls);;CSV files (*.csv)"));
    if (!filename.isEmpty ())
        save_doc (filename);
}

void Application::close ()
{
    m_filename.clear ();
    m_manager.close_graph();
}


void Application::help ()
{
    Helper helper;
    helper.exec ();
}


void Application::open_file (QString filename)
{
    if (filename.isNull () || filename.isEmpty ()) return;

    // prepare data
    m_filename = filename;
    m_manager.load_data (filename.toStdString ());

    //m_spectr.add_data (std::make_shared<PlotData> (
    //    m_source.get_data (tr ("Source row")).get_relative_sp (m_grid.sbBegin->value (), m_grid.sbEnd->value (), m_grid.sbStep->value ())
    //    ));
    static bool flag (true);
    // 
    if (flag)
    {
        connect (m_grid.aSmoothing,     SIGNAL (triggered ()), &m_manager, SLOT (create_smoothing ()));
        connect (m_grid.aApproximation, SIGNAL (triggered ()), &m_manager, SLOT (create_approximation ()));
        connect (m_grid.aDeviations,    SIGNAL (triggered ()), &m_manager, SLOT (create_deviations ()));
        connect (m_grid.aSpectr,        SIGNAL (triggered ()), &m_manager, SLOT (create_spectr ()));
        connect (m_grid.aCorrelation,   SIGNAL (triggered ()), &m_manager, SLOT (create_correlation ()));
        connect (m_grid.aFullWavelet,   SIGNAL (triggered ()), &m_manager, SLOT (create_wavelet ()));
        connect (m_grid.aPow,           SIGNAL (triggered ()), &m_manager, SLOT (create_power ()));
        connect (m_grid.aWaveletSimple, SIGNAL (triggered ()), &m_manager, SLOT (create_s_wavelet ()));

        connect (m_grid.aLogScale,      SIGNAL (triggered ()), SLOT (add_scale_toolbar ()));
        connect (m_grid.aMultyGraph,    SIGNAL (triggered ()), SLOT (add_multygraph_tools ()));
        connect (m_grid.aFill,          SIGNAL (triggered ()), &m_manager, SLOT (remove_spaces ()));
        connect (m_grid.aGrid,          SIGNAL (triggered ()), &m_manager, SLOT (set_grid ()));
        connect (m_grid.aSaveWavlet,    SIGNAL (triggered ()), &m_manager, SLOT (save_plot ()));
        connect (m_grid.aCloseGraph,    SIGNAL (triggered ()), &m_manager, SLOT (close_graph ()));
        flag = false;
    }
}

void Application::save_doc (QString filename)
{
    m_manager.save_data (filename.toStdString ());
}


void Application::add_scale_toolbar ()
{
    if (!logScale)
    {
        logScale = std::make_unique<LogScale> (this);
        addToolBar (logScale.get ());
        m_manager.set_log_scale (logScale.get ());
    }
    else
    {
        removeToolBar (logScale.get ());
        logScale.release ();
    }
    //m_grid.aLogScale->setChecked (logScale);
}

void Application::add_multygraph_tools ()
{
    if (!m_multygraphtools)
    {
        m_multygraphtools = std::make_unique<MultyGraphTools> (this);
        addToolBar (m_multygraphtools.get ());
        m_manager.set_multy_graph (m_multygraphtools.get ());
    }
    else
    {
        removeToolBar (m_multygraphtools.get ());
        m_multygraphtools.release ();
    }
    //m_grid.aMultyGraph->setChecked (m_multygraphtools);
}
