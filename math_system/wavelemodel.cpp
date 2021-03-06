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

#include "wavelemodel.h"
#include "graphmodel.h"

#include <memory>

using std::unique_ptr;


void WaveletModel::calc_wavelet(const GraphModel* const graph, const WaveletInitParams& wavelet_init_params)
{
    const Row& axis (graph->get_axis ());
    StringList slist (graph->get_headers ());
    set_type (graph->get_type ());

    for (std::string name : slist)
    {
        const Row* row (graph->get_source (name));
        if (row)
            add_data (name, new WaveletData (axis, *graph->get_source (name), wavelet_init_params));
        else
            throw std::string ("There were not found any row with such name!");
    }
}

void WaveletModel::save_data (pDocumentWriter document, const std::string& wavelet_desc) const
{
    for (auto& data : m_data)
    {
        data.second->save_data(document->get_page(wavelet_desc + "-" + data.first));
    }
}

const WaveletProducer& WaveletModel::get_data (const std::string& name)
{
    return m_data[name]->get_producer ();
}

void WaveletModel::add_data (const std::string name, WaveletData* data)
{
    m_data[name] = data;
}

StringList WaveletModel::get_headers () const
{
    StringList headers;
    for (const std::pair<std::string, WaveletData*>& pair : m_data)
        headers.push_back(pair.first);
    return headers;
}
