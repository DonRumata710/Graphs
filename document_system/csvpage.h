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
#ifndef CSVPAGE_H
#define CSVPAGE_H


#include "page.h"


class CsvPage : iPage
{
public:
    CsvPage (const std::string &filename);

    virtual bool set_x_axis_type (AxisType type) override;
    virtual bool push_data_back (const std::string& name, const std::vector<double>& data) override;

    virtual AxisType get_x_axis_type () override;

    virtual void get_headers (std::vector<std::string>* const) override;
    virtual void get_data (size_t row, std::vector<double>* const) override;

private:
    bool read_data_to_cache();

private:
    std::string m_filename;
    std::fstream m_file;
    std::vector<std::vector<double>> m_cache;
    std::vector<std::string> m_headers;
};


#endif // CSVPAGE_H
