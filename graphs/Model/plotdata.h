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

#include <functional>
#include <vector>
#include <list>
#include <memory>
#include <string>
using std::shared_ptr;
using std::unique_ptr;
using std::string;

#include "row.h"


using std::shared_ptr;
class QAxObject;


typedef std::vector<string> StringList;


typedef shared_ptr<QPolygonF> pPolygon;

class PlotData
{
    struct PrivateData;

public:
    PlotData ();
    PlotData (const PlotData&);
    PlotData (QAxObject*);
    PlotData (PrivateData* data);

    void set_data (QAxObject*);
    void save_data (QAxObject*) const;

    PlotData& operator= (const PlotData& plotData);

    size_t get_size () const;
    bool empty () const;
    StringList get_headers () const;

    string get_name () const;
    void set_name (string);

    PlotData get_approx () const;
    PlotData get_smoothing (int) const;

    PlotData get_deviations () const;
    PlotData get_relative_sp (double, double, unsigned) const;
    PlotData get_correlations (double, double, unsigned) const;

    PlotData get_power () const;

    void add_row (const Row&);

    const Row& get_axis () const;
    const Row* get_row (string) const;

    void remove_spaces ();

private:
    std::vector<Row>& get_series () const;
    std::vector<double> get_smoothed (const std::vector<double>&) const;
    std::vector<Row>::iterator find_column (string) const;

    shared_ptr<PrivateData> m_data;
};
