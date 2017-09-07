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


PlotManager::PlotManager () {}


void PlotManager::set_tab (QTabWidget* _tab)
{
    tab = _tab;
}


void PlotManager::load_data (const std::string& filename)
{
    size_t index (tab->count ());

    DocumentCreator creator;
    std::unique_ptr<iDocumentReader> document (creator.get_document_reader(filename));

    if (document != nullptr)
    {
        m_pages.push_back (pPresenter (new GraphPresenter (tab, document.get ())));
        tab->setCurrentIndex (index);
    }
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
        for (pPresenter page : m_pages)
        {
            TabPresenter* source = qobject_cast<TabPresenter*> (page);
            if (source)
                source->get_model()->save_data(document.get ());
        }
    }
}

void PlotManager::remove_spaces ()
{
    size_t index (tab->currentIndex ());
    if (m_pages.size () < index) return;
    GraphPresenter* source = qobject_cast<GraphPresenter*> (m_pages[index]);
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
        size_t index (tab->currentIndex ());
        if (m_pages.size () < index)
            return;
        GraphPresenter* source = qobject_cast<GraphPresenter*> (m_pages[index]);
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
        size_t index (tab->currentIndex ());
        if (m_pages.size () < index)
            return;
        GraphPresenter* source = qobject_cast<GraphPresenter*> (m_pages[index]);
        index = tab->count ();
        if (source)
            m_pages.push_back (pPresenter (source->get_deviations ()));
        tab->setCurrentIndex (index);
    }
    catch (ChoiseException) {}
}

void PlotManager::create_smoothing ()
{
    try
    {
        size_t index (tab->currentIndex ());
        if (m_pages.size () < index)
            return;
        GraphPresenter* source (qobject_cast<GraphPresenter*> (m_pages[index]));
        index = tab->count ();
        if (!source)
            return;
        m_pages.push_back (pPresenter (source->get_smoothing ()));
        tab->setCurrentIndex (index);
    }
    catch (ChoiseException) {}
}

void PlotManager::create_spectr ()
{
    try
    {
        size_t index (tab->currentIndex ());
        if (m_pages.size () < index)
            return;
        GraphPresenter* source (qobject_cast<GraphPresenter*> (m_pages[index]));
        index = tab->count ();
        if (!source)
            return;
        m_pages.push_back (pPresenter (source->get_spectr ()));
        tab->setCurrentIndex (index);
    }
    catch (ChoiseException) {}
}

void PlotManager::create_correlation ()
{
    try
    {
        size_t index (tab->currentIndex ());
        if (m_pages.size () < index)
            return;
        GraphPresenter* source = qobject_cast<GraphPresenter*> (m_pages[index]);
        index = tab->count ();
        if (!source)
            return;
        m_pages.push_back (pPresenter (source->get_correlations ()));
        tab->setCurrentIndex (index);
    }
    catch (ChoiseException) {}
}

void PlotManager::create_wavelet ()
{
    try
    {
        size_t index (tab->currentIndex ());
        if (m_pages.size () < index)
            return;
        GraphPresenter* source = qobject_cast<GraphPresenter*> (m_pages[index]);
        index = tab->count ();
        if (!source)
            return;
        m_pages.push_back (pPresenter (source->get_wavelet ()));
        tab->setCurrentIndex (index);
    }
    catch (ChoiseException) {}
}

void PlotManager::create_power ()
{
    size_t index (tab->currentIndex ());
    if (m_pages.size () < index)
        return;
    GraphPresenter* spectr = qobject_cast<GraphPresenter*> (m_pages[index]);
    index = tab->count ();
    if (!spectr)
        return;
    m_pages.push_back (pPresenter (spectr->get_power ()));
    tab->setCurrentIndex (index);
}


void PlotManager::set_multy_graph (MultyGraphTools* tools)
{
    GraphPresenter* c = qobject_cast<GraphPresenter*> (m_pages[tab->currentIndex ()]);
    if (c)
    {
        QStringList list;
        for (string str : c->get_model ()->get_headers ())
            list << QString(str.c_str());
        tools->set_headers (list);
    }
    for (pPresenter pair : m_pages)
    {
        GraphPresenter* c = qobject_cast<GraphPresenter*> (pair);
        c->add_multy_graph_tools (tools);
    }
}

void PlotManager::set_log_scale (LogScale* logScale)
{
    for (auto pair : m_pages)
        pair->set_scale_toolbar (logScale);
}

void PlotManager::set_grid ()
{
    for (pPresenter pair : m_pages)
        pair->set_grid ();
}


void PlotManager::create_s_wavelet ()
{
    try
    {
        size_t index (tab->currentIndex ());
        if (m_pages.size () < index)
            return;
        SpectrogramPresenter* raster = qobject_cast<SpectrogramPresenter*> (m_pages[index]);
        index = tab->count ();
        if (!raster)
            return;
        m_pages.push_back (pPresenter (raster->get_local_wavlet ()));
        tab->setCurrentIndex (index);
    }
    catch (ChoiseException) {}
}


void PlotManager::save_plot ()
{
    m_pages[tab->currentIndex ()]->save_picture (tr ("plot.pdf"));
}

void PlotManager::close_graph ()
{
    tab->clear ();
    m_pages.clear ();
}

void PlotManager::close_graph (int i)
{
    tab->removeTab (i);
    auto tmp (m_pages[i]);
    m_pages[i] = m_pages[m_pages.size() - 1];
    m_pages[m_pages.size() - 1] = tmp;
    m_pages.erase (m_pages.end () - 1);
}
