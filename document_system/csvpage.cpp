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


#include "csvpage.h"
#include <sstream>


CsvPage::CsvPage(const std::string& filename) : m_filename (filename)
{}

CsvPage::~CsvPage()
{
    save_file ();
}

bool CsvPage::set_x_axis_type(AxisType type)
{
    return true;
}

bool CsvPage::push_data_back(const std::string& name, const std::vector<double>& data)
{
    if (m_cache.empty ())
        m_cache.assign(data.size () + 1, std::vector<std::string> ());

    m_cache[0].push_back (name);

    for (size_t i = 0; i != data.size (); ++i)
        m_cache[i + 1].push_back (std::to_string (data[i]));

    return true;
}

bool CsvPage::push_data_back(const std::string &name, const double *data, size_t size)
{
    if (m_cache.empty ())
        m_cache.assign(size + 1, std::vector<std::string> ());

    m_cache[0].push_back (name);

    for (size_t i = 0; i != size; ++i)
        m_cache[i + 1].push_back (std::to_string (data[i]));

    return true;
}

AxisType CsvPage::get_x_axis_type()
{
    if (m_cache.empty ())
    {
        if (!read_file (';'))
            return AxisType::TYPE_NUM;
    }
    return get_str_type (m_cache[1][0]);
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

bool CsvPage::get_data(size_t index, std::vector<double>* const data)
{
    if (!data)
        return false;

    data->clear ();

    if (m_cache.empty ())
    {
        if (!read_file (';'))
            return false;
    }

    if (index == 0 && get_x_axis_type () == AxisType::TYPE_TIME)
    {
        for (const std::vector<std::string>& row : m_cache)
        {
            try
            {
                brouse_date_formats (data, row[0]);
            }
            catch (const std::invalid_argument&)
            {}
        }
        return true;
    }

    for (const std::vector<std::string>& val : m_cache)
    {
        try
        {
            data->push_back (string_to_double (val[index]));
        }
        catch (const std::invalid_argument&)
        {}
    }
    return true;
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

    m_cache.push_back (std::vector<std::string> ());
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
            }

            m_cache.back ().push_back (ss.str ());
            ss.str ("");
            m_cache.push_back (std::vector<std::string> ());
        }
        else
        {
            ss << c;
        }
    }

    if (m_cache.back ().empty ())
        m_cache.pop_back ();

    return true;
}

void CsvPage::brouse_date_formats(std::vector<double>* data, const std::string& str)
{
    QDateTime date (QDateTime::fromString (str.c_str (), "dd.MM.yy H:mm"));
    if (date.isValid ())
    {
        data->push_back (date_to_double (date));
    }

    date = QDateTime::fromString (str.c_str (), Qt::TextDate);
    if (date.isValid ())
    {
        data->push_back (date_to_double (date));
    }

    date = QDateTime::fromString (str.c_str (), Qt::ISODate);
    if (date.isValid ())
    {
        data->push_back (date_to_double (date));
    }

    date = QDateTime::fromString (str.c_str (), Qt::SystemLocaleShortDate);
    if (date.isValid ())
    {
        data->push_back (date_to_double (date));
    }

    date = QDateTime::fromString (str.c_str (), Qt::SystemLocaleLongDate);
    if (date.isValid ())
    {
        data->push_back (date_to_double (date));
    }

    date = QDateTime::fromString (str.c_str (), Qt::DefaultLocaleShortDate);
    if (date.isValid ())
    {
        data->push_back (date_to_double (date));
    }

    date = QDateTime::fromString (str.c_str (), Qt::DefaultLocaleLongDate);
    if (date.isValid ())
    {
        data->push_back (date_to_double (date));
    }

    date = QDateTime::fromString (str.c_str (), Qt::SystemLocaleDate);
    if (date.isValid ())
    {
        data->push_back (date_to_double (date));
    }

    date = QDateTime::fromString (str.c_str (), Qt::LocaleDate);
    if (date.isValid ())
    {
        data->push_back (date_to_double (date));
    }

    date = QDateTime::fromString (str.c_str (), Qt::LocalDate);
    if (date.isValid ())
    {
        data->push_back (date_to_double (date));
    }

    date = QDateTime::fromString (str.c_str (), Qt::RFC2822Date);
    if (date.isValid ())
    {
        data->push_back (date_to_double (date));
    }
}

double CsvPage::date_to_double(const QDateTime& date)
{

    double days (abs (date.daysTo (QDateTime (QDate (1899, 12, 30), QTime ()))));
    days += (date.time ().msecsSinceStartOfDay () / 60000) / 1440.0;
    return days;
}

double CsvPage::string_to_double(const std::string& str)
{
    std::string number (str);
    size_t pos (str.find (','));
    if (pos != std::string::npos)
    {
        number.replace (pos, 1, ".");
    }
    return std::stod (number);
}

void CsvPage::save_file()
{
    std::ofstream file (m_filename);
    if (file.is_open ())
    {
        for (const std::vector<std::string>& line : m_cache)
        {
            for (const std::string& value : line)
                file << value << ";";
            file << "\n";
        }
    }
}
