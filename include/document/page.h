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

#ifndef PAGE_H
#define PAGE_H


#include "axistype.h"

#include <string>
#include <vector>
#include <memory>


class iPage
{
public:
    virtual ~iPage () {}

    virtual bool set_x_axis_type (AxisType type) = 0;
    virtual bool push_data_back (const std::string& name, const std::vector<double>& data) = 0;
    virtual bool push_data_back (const std::string& name, const double* data, size_t size) = 0;

    virtual AxisType get_x_axis_type () = 0;

    virtual bool get_headers (std::vector<std::string>* const) = 0;
    virtual bool get_data (size_t row, std::vector<double>* const) = 0;

protected:
    static AxisType get_str_type (const std::string& str)
    {
        size_t pos (0);
        try
        {
            std::stod (str, &pos);
        }
        catch (const std::invalid_argument&)
        {
            return TYPE_TIME;
        }

        for (size_t i = pos; i < str.size (); ++i)
        {
            if (isdigit (str[i]))
                return TYPE_TIME;
        }

        return TYPE_NUM;
    }

    static bool is_text (const std::string& str)
    {
        return !(isdigit (*str.begin ()) && (isdigit (*(str.end () - 1))));
    }
};


typedef std::shared_ptr<iPage> pPage;


#endif
