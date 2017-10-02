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


#include "document/documentreader.h"
#include "document/documentwriter.h"

#include <functional>
#include <vector>
#include <list>
#include <string>

#include "row.h"


typedef std::vector<std::string> StringList;


class PlotData
{
public:
    PlotData ();
    PlotData (const PlotData&);
    PlotData (pDocumentReader);

    void load_data (pPage page);
    void save_data (pPage page) const;

    PlotData& operator= (const PlotData& plotData);

    size_t get_size () const;
    bool empty () const;
    StringList get_headers () const;

    const std::string& get_name () const;
    void set_name (const std::string& name);

    PlotData get_approx () const;
    PlotData get_smoothing (int) const;

    PlotData get_deviations () const;
    PlotData get_relative_sp (double, double, unsigned) const;
    PlotData get_correlations (double, double, unsigned) const;

    PlotData get_power () const;

    void add_row (const Row&);

    const Row& get_axis () const;
    const Row* get_row (const std::string& row_name) const;

    void remove_spaces ();

private:
    struct PrivateData;
    typedef std::shared_ptr<PrivateData> pPrivateData;

    typedef std::vector<Row>::iterator iterRow;

    PlotData (pPrivateData data);

    std::vector<double> get_smoothed (const std::vector<double>&) const;
    iterRow find_column (const std::string&) const;

    iterRow quick_search (const iterRow& begin, const iterRow& end, const std::string& col) const;

    pPrivateData m_data;
};
