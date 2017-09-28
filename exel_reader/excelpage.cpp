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


#include "excelpage.h"
#include "exceldocumentwriter.h"

#include <ActiveQt/qaxobject.h>
#include <ActiveQt/qaxbase.h>


ExcelPage::~ExcelPage()
{
    try
    {
        if (!m_table)
            return;

        std::unique_ptr<QAxObject> first_cell (m_table->querySubObject (
            "Cells(QVariant&,QVariant&)",
            QVariant (1),
            QVariant (1)
        ));
        std::unique_ptr<QAxObject> second_cell (m_table->querySubObject (
            "Cells(QVariant&,QVariant&)",
            QVariant (m_cache.size ()),
            QVariant (m_cache[0].size ())
        ));

        if (!first_cell || !second_cell)
            return;

        std::unique_ptr<QAxObject> range (m_table->querySubObject (
            "Range(const QVariant&,const QVariant&)",
            first_cell->asVariant (),
            second_cell->asVariant ()
        ));

        if (!range)
            return;

        QList<QVariant> result;
        for (QList<QVariant> list : m_cache)
            result.push_back (list);

        range->setProperty ("Value", QVariant (result));
    }
    catch (...)
    {}
}

bool ExcelPage::set_x_axis_type(AxisType type)
{
    return true;
}

bool ExcelPage::push_data_back(const std::string& name, const std::vector<double>& data)
{
    if (m_cache.size () < data.size ())
        m_cache.assign(data.size () + 1, QList<QVariant> ());

    m_cache[0] << name.c_str ();
    for (size_t i = 0; i < data.size (); ++i)
    {
        m_cache[i + 1] << data[i];
    }

    return true;
}

AxisType ExcelPage::get_x_axis_type()
{
    std::unique_ptr<QAxObject> cell (m_table->querySubObject (
        "Cells(QVariant,QVariant)", QVariant (2), QVariant (1)
    ));
    return get_str_type (cell->property ("Value").toString ().toStdString ());
}

bool ExcelPage::get_headers(std::vector<std::string>* const headers)
{
    if (!headers)
        return false;

    if (m_cache.empty ())
    {
        if (!load_data ())
            return false;
    }

    headers->clear ();
    headers->reserve(m_columns_num);

    if (m_have_headers)
    {
        QList<QVariant> header_list (m_cache.front ());
        QList<QVariant>::iterator iter (header_list.begin ());
        while (iter != header_list.end ())
            headers->push_back ((iter++)->toString ().toStdString ());
    }
    else
    {
        for (size_t i = 0; i < m_columns_num; ++i)
            headers->push_back (std::to_string (i));
    }

    return true;
}

bool ExcelPage::get_data(size_t index, std::vector<double>* const row)
{
    if (!row)
        return false;

    if (m_cache.empty ())
    {
        if (!load_data ())
            return false;
    }

    if (index > m_cache.size ())
        return false;

    row->clear ();
    row->reserve(m_points_num - (m_have_headers ? 1 : 0));

    std::vector<QList<QVariant>>::iterator iter (m_cache.begin () + (m_have_headers ? 1 : 0));
    try
    {
        while (iter != m_cache.end ())
            row->push_back ((*iter++).at (index).toDouble ());
    }
    catch (...)
    {}
    return true;
}

ExcelPage::ExcelPage(std::shared_ptr<ExcelFile> file, QAxObject* table) :
    m_file (file),
    m_table (table)
{}

ExcelPage::ExcelPage(std::shared_ptr<ExcelFile> file, std::unique_ptr<QAxObject> &&table) :
    m_file (file),
    m_table (std::move (table))
{}

size_t ExcelPage::get_rows_number()
{
    if (!m_columns_num)
    {
        std::unique_ptr<QAxObject> cell (m_table->querySubObject ("Cells(QVariant,Columns.Count)", QVariant (1)));
        if (!cell)
            return 0;
        std::unique_ptr<QAxObject> columns_num (cell->querySubObject ("End (xlToRight)"));
        if (!columns_num)
            return 0;
        m_columns_num = columns_num->property ("Column").toInt ();
    }
    return m_columns_num;
}

size_t ExcelPage::get_points_number()
{
    if (!m_points_num)
    {
        std::unique_ptr<QAxObject> cell (m_table->querySubObject ("Cells(Rows.Count,QVariant)", QVariant (1)));
        if (!cell)
            return 0;
        std::unique_ptr<QAxObject> numRowsEx (cell->querySubObject ("End (xlDown)"));
        if (!numRowsEx)
            return 0;
        m_points_num = numRowsEx->property ("Row").toInt ();
    }
    return m_points_num;
}

bool ExcelPage::load_data ()
{
    if (!m_table)
        return false;

    std::unique_ptr<QAxObject> top_left_cell (m_table->querySubObject ("Cells(QVariant&,QVariant&)", QVariant (1), QVariant (1)));
    std::unique_ptr<QAxObject> bottom_right_cell (m_table->querySubObject ("Cells(QVariant&,QVariant&)", QVariant (get_points_number ()), QVariant (get_rows_number ())));

    if (!top_left_cell || !bottom_right_cell)
        return false;

    std::unique_ptr<QAxObject> range (m_table->querySubObject (
        "Range(const QVariant&,const QVariant&)", top_left_cell->asVariant (), bottom_right_cell->asVariant ()
    ));

    if (!range)
        return false;

    range->setProperty ("NumberFormat", QVariant ("Double"));

    QList<QVariant> cache_list (range->property ("Value").toList ());

    for (QVariant list : cache_list)
        m_cache.push_back (list.toList ());

    m_have_headers = is_text (m_cache.at (0).at (1).toString ().toStdString ());
    return true;
}
