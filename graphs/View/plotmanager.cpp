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


#include "Presenter/graphpresenter.h"
#include "math/graphmodel.h"

#include "Presenter/spectrogrampresenter.h"
#include "math/wavelemodel.h"

#include "choiseexception.h"
#include "multygraph.h"

#include "plotmanager.h"

#include "document/documentcreator.h"

#include <iostream>



template<typename T> class Deleter
{
public:
    Deleter (T* res) : m_res (res)
    {}

    Deleter(Deleter&& other) : m_res (other.m_res)
    {
        other.m_res = nullptr;
    }

    ~Deleter()
    {
        if (m_res)
            delete m_res;
    }

private:
    Deleter(const Deleter&);

    T* m_res;
};



PlotManager::PlotManager()
{}

void PlotManager::set_tab (QTabWidget* _tab)
{
    m_tab = _tab;
}

void PlotManager::load_data (const std::string& filename)
{
    new GraphPresenter (m_tab, filename);
}

void PlotManager::save_data (const std::string& filename)
{
    //QAxObject* sheet (sheets->querySubObject ("Item (const QVariant&)", QVariant (1)));
    //
    //while (sheet)
    //{
    //    sheet->dynamicCall ("Delete");
    //    delete sheet;
    //    sheet = sheets->querySubObject ("Item (const QVariant&)", QVariant (1));
    //}

    DocumentCreator creator;
    std::unique_ptr<iDocumentWriter> document (creator.get_document_writer(filename));

    if (document != nullptr)
    {
        for (size_t i = 0; i < m_tab->count (); ++i)
        {
            TabPresenter* source = qobject_cast<TabPresenter*> (m_tab->widget (i));
            if (source)
                source->get_model()->save_data(document.get ());
        }
    }
}

void PlotManager::remove_spaces ()
{
    GraphPresenter* source = qobject_cast<GraphPresenter*> (m_tab->currentWidget ());
    if (source)
    {
        source->get_model ()->remove_spaces ();
        return;
    }
}


void PlotManager::create_approximation ()
{
    try
    {
        GraphPresenter* source = qobject_cast<GraphPresenter*> (m_tab->currentWidget ());
        if (source)
        {
            source->get_model ()->set_approximation ();
            return;
        }
        return;
    }
    catch (ChoiseException) {}
}

void PlotManager::create_deviations ()
{
    try
    {
        GraphPresenter* source = qobject_cast<GraphPresenter*> (m_tab->currentWidget ());
        if (source)
            source->get_deviations ();
    }
    catch (ChoiseException) {}
}

void PlotManager::create_smoothing ()
{
    try
    {
        GraphPresenter* source (qobject_cast<GraphPresenter*> (m_tab->currentWidget ()));
        if (!source)
            return;
        source->get_smoothing ();
    }
    catch (ChoiseException) {}
}

void PlotManager::create_spectr ()
{
    try
    {
        GraphPresenter* source (qobject_cast<GraphPresenter*> (m_tab->currentWidget ()));
        if (!source)
            return;
        source->get_spectr ();
    }
    catch (ChoiseException) {}
}

void PlotManager::create_correlation ()
{
    try
    {
        GraphPresenter* source = qobject_cast<GraphPresenter*> (m_tab->currentWidget ());
        if (!source)
            return;
        source->get_correlations ();
    }
    catch (ChoiseException) {}
}

void PlotManager::create_wavelet ()
{
    try
    {
        GraphPresenter* source = qobject_cast<GraphPresenter*> (m_tab->currentWidget ());
        if (!source)
            return;
        source->get_wavelet ();
    }
    catch (ChoiseException) {}
}

void PlotManager::create_power ()
{
    GraphPresenter* spectr = qobject_cast<GraphPresenter*> (m_tab->currentWidget ());
    if (!spectr)
        return;
    spectr->get_power ();
}


void PlotManager::set_multy_graph (MultyGraphTools* tools)
{
    GraphPresenter* c = qobject_cast<GraphPresenter*> (m_tab->currentWidget ());
    if (c)
    {
        QStringList list;
        for (std::string str : c->get_model ()->get_headers ())
            list << QString(str.c_str());
        tools->set_headers (list);
    }
    for (size_t i = 0; i < m_tab->count (); ++i)
    {
        GraphPresenter* c = qobject_cast<GraphPresenter*> (m_tab->currentWidget ());
        c->add_multy_graph_tools (tools);
    }
}

void PlotManager::set_log_scale (LogScale* logScale)
{
    for (size_t i = 0; i < m_tab->count (); ++i)
        qobject_cast<TabPresenter*> (m_tab->widget(i))->set_scale_toolbar (logScale);
}

void PlotManager::set_grid ()
{
    for (size_t i = 0; i < m_tab->count (); ++i)
        qobject_cast<TabPresenter*> (m_tab->widget(i))->set_grid ();
}


void PlotManager::create_s_wavelet ()
{
    try
    {
        SpectrogramPresenter* raster = qobject_cast<SpectrogramPresenter*> (m_tab->currentWidget ());
        if (!raster)
            return;
        raster->get_local_wavlet ();
    }
    catch (ChoiseException) {}
}


void PlotManager::save_plot ()
{
    qobject_cast<TabPresenter*> (m_tab->currentWidget ())->save_picture (tr ("plot.pdf"));
}

void PlotManager::close_graph ()
{
    std::list<Deleter<QWidget>> deleters;
    for (size_t i = 0; i < m_tab->count (); ++i)
        deleters.push_back (Deleter<QWidget> (m_tab->widget (i)));
    m_tab->clear ();
}

void PlotManager::close_graph (int i)
{
    Deleter<QWidget> deleter (m_tab->widget (i));
    m_tab->removeTab (i);
}
