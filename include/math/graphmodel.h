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

#include "plotdata.h"
#include "abstractmodel.h"


class GraphModel : public AbstractModel
{
public:
    GraphModel () {}

    GraphModel (pDocument doc)
    {
        load_data (doc);
    }

    virtual ~GraphModel () {}

    void set_approximation ()
    {
        if (m_approx.empty ())
            m_approx = m_source.get_approx ();
    }

    void add_row (const Row& row)
    {
        m_source.add_row (row);
    }

    GraphModel* get_deviations () const { return new GraphModel (m_source.get_deviations ()); }
    GraphModel* get_smoothing (int points) const { return new GraphModel (m_source.get_smoothing (points)); }
    GraphModel* get_correlations (double begin, double end, unsigned step) const;
    GraphModel* get_relative_sp (double begin, double end, unsigned step) const;
    GraphModel* get_power () const;

    void save_data (pDocument document) const;
    void load_data (pDocument document);

    void remove_spaces ()
    {
        m_source.remove_spaces ();
        m_approx.remove_spaces ();
    }
    
    const Row& get_axis () const
    {
        return m_source.get_axis ();
    }

    const Row* get_source (string name) const
    {
        return m_source.get_row (name);
    }

    const Row& get_approx_axis () const
    {
        return m_approx.get_axis ();
    }

    const Row* get_approx (string name) const
    {
        if (m_approx.empty ()) return nullptr;
        return m_approx.get_row (name);
    }

    virtual StringList get_headers () const override;
    virtual string get_name () const override;

private:
    GraphModel (PlotData plotData) : m_source (plotData) {}

    PlotData m_source;
    PlotData m_approx;
};
