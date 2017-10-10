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


#include "graphmodel.h"


GraphModel *GraphModel::get_deviations() const
{
    return new GraphModel (m_source.get_deviations ());
}

GraphModel *GraphModel::get_smoothing(int points) const
{
    return new GraphModel (m_source.get_smoothing (points));
}

GraphModel* GraphModel::get_correlations (double begin, double end, unsigned step) const
{
    return new GraphModel (m_source.get_correlations (begin, end, step));
}

GraphModel* GraphModel::get_relative_sp (double begin, double end, unsigned step) const
{
    return new GraphModel (m_source.get_relative_sp (begin, end, step));
}

GraphModel* GraphModel::get_power () const
{
    return new GraphModel (m_source.get_power ());
}

void GraphModel::load_data(pPage page)
{
    set_type(page->get_x_axis_type ());
    m_source.load_data (page);
}

void GraphModel::save_data(pPage page) const
{
    page->set_x_axis_type(get_type ());
    m_source.save_data (page);
}

void GraphModel::remove_spaces()
{
    m_source.remove_spaces ();
    m_approx.remove_spaces ();
}

const Row *GraphModel::get_approx(std::string name) const
{
    if (m_approx.empty ())
        return nullptr;
    return m_approx.get_row (name);
}

StringList GraphModel::get_headers() const
{
    return m_source.get_headers ();
}
