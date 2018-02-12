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


#include <QtCore>
#include <QtWidgets>

#include <memory>
#include <deque>


class QAxObject;
class TabPresenter;
class LogScale;
class MultyGraphTools;
typedef TabPresenter* pPresenter;


class PlotManager : public QObject
{
    Q_OBJECT

public:
    PlotManager ();

    void set_widgets (QTabWidget*, QStatusBar*);

    void load_data (const std::string& document);
    void save_data (const std::string& document);

public slots:
    void remove_spaces ();

    void create_approximation ();
    void create_deviations ();
    void create_smoothing ();
    void create_spectr ();
    void create_correlation ();
    void create_wavelet ();
    void create_power ();

    void create_s_wavelet ();

    void set_multy_graph (MultyGraphTools*);
    void set_log_scale (LogScale*);
    void set_grid ();

    void save_plot ();
    void close_graph ();
    void close_graph (int);

private:
    PlotManager (const PlotManager&);

    QTabWidget* m_tab;
    QStatusBar* m_status_bar;
};
