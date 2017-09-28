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


#include "csvpage.h"
#include <sstream>


CsvPage::CsvPage(const std::string& filename) : m_filename (filename)
{}

bool CsvPage::set_x_axis_type(AxisType type)
{
    return true;
}

bool CsvPage::push_data_back(const std::string& name, const std::vector<double>& data)
{
    m_cache[0].push_back (name);

    for (size_t i = 0; i != data.size (); ++i)
        m_cache[i].push_back (std::to_string (data[i]));

    return true;
}

AxisType CsvPage::get_x_axis_type()
{
    return AxisType::TYPE_NUM;
}

bool CsvPage::get_headers(std::vector<std::string>* const headers)
{
    if (!headers)
        return false;

    headers->clear ();

    if (m_cache.empty ())
    {
        if (!read_file (';'))
            return false;
    }

    if (!is_text (m_cache[0].back ()))
    {
        for (size_t i = 0; i < m_cache[0].size (); ++i)
            headers->push_back (std::to_string (i));
    }
    else
    {
        *headers = m_cache[0];
    }

    return true;
}

bool CsvPage::get_data(size_t row, std::vector<double>* const data)
{
    if (!data)
        return false;

    data->clear ();

    if (m_cache.empty ())
    {
        if (!read_file (';'))
            return false;
    }

    for (const std::string& val : m_cache[0])
        data->push_back (std::stod (val));
}

bool CsvPage::read_file (char delimiter)
{
    std::stringstream ss;
    bool inquotes = false;

    if (m_file.is_open ())
        m_file.seekg (0, m_file.beg);
    else
        m_file.open (m_filename);

    if (!m_file.is_open ())
        return false;

    while (m_file.good ())
    {
        char c = m_file.get ();
        if (!inquotes && c == '"')
        {
            inquotes = true;
        }
        else if (inquotes && c == '"')
        {
            if (m_file.peek () == '"')
                ss << (char) m_file.get ();
            else
                inquotes = false;
        }
        else if (!inquotes && c == delimiter)
        {
            m_cache.back ().push_back (ss.str ());
            ss.str ("");
        }
        else if (!inquotes && (c == '\r' || c == '\n'))
        {
            if (m_file.peek () == '\n')
            {
                m_file.get ();
                m_cache.push_back (std::vector<std::string> ());
            }

            m_cache.back ().push_back (ss.str ());
        }
        else
        {
            ss << c;
        }
    }

    return true;
}
