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

#include "graphpresenter.h"
#include "spectrogrampresenter.h"

#include "document/documentcreator.h"

#include "math/row.h"
#include "math/graphmodel.h"
#include "math/wavelemodel.h"

#include "View/multygraph.h"
#include "View/smoothing.h"
#include "View/spectr.h"
#include "View/wavelet.h"
#include "View/choiseexception.h"

#include "qwt_plot.h"


GraphPresenter::GraphPresenter (QStatusBar* status_bar) :
    TabPresenter (status_bar)
{}

GraphPresenter::GraphPresenter (QTabWidget* parent, const std::string& filename, QStatusBar* status_bar) :
    TabPresenter (status_bar)
{
    if (parent)
    {
        TabPresenter::init (parent, new GraphModel);
        std::string::size_type start (filename.find_last_of('/') + 1);
        std::string::size_type end (filename.find_last_of('.'));

        if (start > end)
            start = 0;

        std::string name (filename.substr (start, end));
        set_name (name);

        m_thread.set_func ([=](){
            DocumentCreator creator;
            std::unique_ptr<iDocumentReader> document (creator.get_document_reader(filename));

            if (!document)
                return;

            get_model ()->load_data (document->get_page (0));

            QStringList list;
            for (std::string str : get_model ()->get_headers ())
                list << QString (str.c_str ());

            m_source->addItems (list);
            set_x_format (get_model ()->get_type ());
        });
        connect (&m_thread, SIGNAL(completed()), SLOT(loading_complete()));
        m_thread.start ();
    }
}

void GraphPresenter::add_multy_graph_tools (MultyGraphTools* tools)
{
    tools->connect_widget (this);
}

GraphModel* GraphPresenter::get_model () const
{
    return (GraphModel*) TabPresenter::get_model ();
}

void GraphPresenter::create_deviations_row() const
{
    GraphPresenter* presenter (new GraphPresenter (get_status_bar ()));
    presenter->set_name (get_name () + " - deviations row");
    connect (&m_thread, SIGNAL(completed()), presenter, SLOT(loading_complete()));

    m_thread.set_func ([this, presenter](){
        presenter->init (get_tab (), get_model ()->get_deviations ());
    });
    m_thread.start ();
}

void GraphPresenter::create_smoothing () const
{
    Smoothing smoothing;
    if (smoothing.exec () != QDialog::Accepted)
        throw ChoiseException ();

    int value = smoothing.get_value ();

    GraphPresenter* presenter (new GraphPresenter (get_status_bar ()));
    presenter->set_name (get_name () + " - smoothed row by " + std::to_string(value) + " points");
    connect (&m_thread, SIGNAL(completed()), presenter, SLOT(loading_complete()));

    m_thread.set_func ([this, presenter, value](){
        presenter->init (get_tab (), get_model ()->get_smoothing (value));
    });
    m_thread.start ();
}

void GraphPresenter::create_spectr() const
{
    Spectr spectr;

    if (spectr.exec () != QDialog::Accepted)
        throw ChoiseException ();

    GraphPresenter* presenter (new GraphPresenter (get_status_bar ()));
    connect (&m_thread, SIGNAL(completed()), presenter, SLOT(loading_complete()));

    double begin (spectr.get_begin ());
    double end (spectr.get_end ());
    double steps (spectr.get_steps ());

    m_thread.set_func ([this, presenter, begin, end, steps](){
        presenter->init (get_tab (), get_model ()->get_relative_sp (begin, end, steps));
    });
    m_thread.start ();
}

void GraphPresenter::create_correlations () const
{
    Spectr spectr;

    if (spectr.exec () != QDialog::Accepted)
        throw ChoiseException ();

    GraphPresenter* presenter (new GraphPresenter (get_status_bar ()));
    presenter->set_name (get_name () + " - correlations");
    connect (&m_thread, SIGNAL(completed()), presenter, SLOT(loading_complete()));

    double begin (spectr.get_begin ());
    double end (spectr.get_end ());
    double steps (spectr.get_steps ());

    m_thread.set_func ([this, presenter, begin, end, steps](){
        presenter->init (get_tab (), get_model ()->get_correlations (begin, end, steps));
    });
    m_thread.start ();
}

void GraphPresenter::create_power_spectr () const
{
    GraphPresenter* presenter (new GraphPresenter (get_status_bar ()));
    presenter->set_name (get_name () + " - spectr");
    connect (&m_thread, SIGNAL(completed()), presenter, SLOT(loading_complete()));

    m_thread.set_func ([this, presenter](){
        presenter->init (get_tab (), get_model ()->get_power ());
    });
    m_thread.start ();
}

void GraphPresenter::create_wavelet () const
{
    WaveletWindow waveletWin;
    if (waveletWin.exec () != QDialog::Accepted)
        throw ChoiseException ();

    new SpectrogramPresenter (get_tab (), get_model (), waveletWin.get_wavelet_info (), get_status_bar (), get_name ());
}

void GraphPresenter::prepare_tab()
{
    connect (
        get_headers (),
        SIGNAL (currentTextChanged (const QString&)),
        SLOT (attach_curve (const QString&))
    );
    attach_curve (get_headers ()->currentText ());
    get_tab ()->addTab (this, QString(get_name ().c_str()));
}

void GraphPresenter::set_current_curve (const QString& cur)
{
    m_current = cur;
}

void GraphPresenter::attach_curve ()
{
    if (m_current.isEmpty ())
        return;

    const Row* samples (get_model ()->get_source (m_current.toStdString()));
    if (samples)
        add_curve (get_model ()->get_axis (), *samples, m_current);

    samples = get_model ()->get_approx (m_current.toStdString());
    if (samples)
        add_curve (get_model ()->get_approx_axis (), *samples, m_current);

    get_plot ()->replot ();
}

void GraphPresenter::detach_curve ()
{
    if (!m_current.isEmpty ()) detach_curve (m_current);
}


void GraphPresenter::attach_curve (const QString& name)
{
    for (pCurve p : m_curves) if (p->get_name () == name) return;

    clear ();

    const Row* samples (get_model ()->get_source (name.toStdString()));
    if (samples) add_curve (get_model ()->get_axis (), *samples, name);

    samples = get_model ()->get_approx (name.toStdString());
    if (samples) add_curve (get_model ()->get_approx_axis (), *samples, name);

    get_plot ()->replot ();
}

void GraphPresenter::detach_curve (const QString& name)
{
    size_t i (0);
    for (; i < m_curves.size (); ++i)
        if (m_curves[i]->get_name () == name)
        {
            m_curves[i]->detach ();
            m_curves.erase (m_curves.begin () + i);
            break;
        }

    for (; i < m_curves.size (); ++i)
        switch (i)
        {
            case 1:
                m_curves[i]->setPen (Qt::GlobalColor::black, 0.0, Qt::PenStyle::SolidLine);
                break;
            case 2:
                m_curves[i]->setPen (Qt::GlobalColor::black, 0.0, Qt::PenStyle::DashLine);
                break;
            case 3:
                m_curves[i]->setPen (Qt::GlobalColor::black, 0.0, Qt::PenStyle::DotLine);
                break;
            case 4:
                m_curves[i]->setPen (Qt::GlobalColor::black, 0.0, Qt::PenStyle::DashDotDotLine);
                break;
            default:
                break;
        }

    get_plot ()->replot ();
    --m_count;
}

void GraphPresenter::clear ()
{
    for (pCurve p : m_curves) p->detach ();
    m_curves.clear ();
    get_plot ()->replot ();
    m_count = 0;
}

void GraphPresenter::add_curve (const vector<double>& axis, const vector<double>& samples, const QString name)
{
    ++m_count;

    pCurve p = std::make_shared<Curve> (&axis[0], &samples[0], axis.size (), name);
    
    switch (m_count)
    {
    case 1:
        p->setPen (Qt::GlobalColor::black, 0.0, Qt::PenStyle::SolidLine);
        break;
    case 2:
        p->setPen (Qt::GlobalColor::black, 0.0, Qt::PenStyle::SolidLine);
        break;
    case 3:
        p->setPen (Qt::GlobalColor::black, 0.0, Qt::PenStyle::SolidLine);
        break;
    case 4:
        p->setPen (Qt::GlobalColor::black, 0.0, Qt::PenStyle::SolidLine);
        break;
    default:
        break;
    }
    p->attach (get_plot ());
    m_curves.insert (m_curves.end (), p);
}
