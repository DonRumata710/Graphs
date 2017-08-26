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


#include <string>
using std::string;


///////////////////////////////////////////////////////////////////////
class Row : public std::vector<double>
{
public:
    explicit Row (string name = "") : m_name (name) {}
    Row (string name, size_t size, double val) : std::vector<double> (size, val), m_name (name) {}
    //Row (string name, QVector<double>& values) : std::vector<double> (values), m_name (name) {}
    //Row (string name, QVector<double>&& values) : std::vector<double> (values), m_name (name) {}

    Row (const Row& values) : std::vector<double> (values), m_name (values.m_name) {}
    Row (const Row&& values) : std::vector<double> (std::move (values)), m_name (std::move (values.m_name)) {}

    Row& operator= (const Row& values)
    {
        assign(values.begin(), values.end());
        m_name = values.m_name;
        return *this;
    }

    bool    operator<  (const string& row) const  { return m_name < row; }
    bool    operator<  (const Row& row) const      { return m_name < row.m_name; }

    bool    operator== (const string& name) const { return m_name == name; }
    bool    operator== (const Row& row) const      { return m_name == row.m_name; }

    string get_name () const                      { return m_name; }

private:
    string m_name;
};
////////////////////////////////////////////////////////////


bool operator< (const QString&str, const Row& col);
