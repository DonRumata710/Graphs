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

#include "qapplication.h"
#include "qlayout.h"
#include "qwt_plot.h"
#include "qwt_plot_marker.h"
#include "qwt_plot_curve.h"
#include "qwt_legend.h"
#include "qwt_point_data.h"
#include "qwt_plot_canvas.h"
#include "qwt_plot_panner.h"
#include "qwt_plot_magnifier.h"
#include "qwt_plot_spectrogram.h"
#include "qwt_scale_div.h"
#include "qwt_series_data.h"
#include "qwt_text.h"
#include "qwt_symbol.h"
#include "qwt_math.h"
#include "math.h"

#include "ui_main_win.h"

#include "plotmanager.h"


class Application : public QMainWindow {
    Q_OBJECT

public:
    Application (QWidget *parent = 0);

public slots:
    // file
    void open ();
    void save ();
    void save_as ();
    void close ();

    // help
    void help ();

    //
    void set_filename (const QString filename);

    void add_scale_toolbar ();
    void add_multygraph_tools ();

private:
    void redraw ();

    void open_file (QString);
    void save_doc (QString filename);

    QString m_filename;

    Ui_GraphsClass m_grid;

    PlotManager m_manager;

    LogScale* logScale = nullptr;
    MultyGraphTools* m_multygraphtools = nullptr;
};
