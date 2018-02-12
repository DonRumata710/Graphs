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


#include "row.h"


Row::Row()
{}

Row::Row(const std::string& name) : m_name (name)
{}

Row::Row(const std::string& name, size_t size) :
    std::vector<double> (size),
    m_name (name)
{}

Row::Row(const std::string& name, size_t size, double value) :
    std::vector<double> (size, value),
    m_name (name)
{}

Row::Row(const std::string& name, const std::vector<double>& data) :
    std::vector<double> (data),
    m_name (name)
{}

Row::Row(const std::string& name, std::vector<double>&& data) :
    std::vector<double> (std::move (data)),
    m_name (name)
{}

Row::Row (const Row& values) :
    std::vector<double> (values),
    m_name (values.m_name)
{}

Row::Row (Row&& values) :
    std::vector<double> (std::move (values)),
    m_name (std::move (values.m_name))
{}

Row &Row::operator=(const Row &values)
{
    assign(values.begin(), values.end());
    m_name = values.m_name;
    return *this;
}

bool operator<(const std::string &str, const Row &col)
{
    return str < col.get_name ();
}
