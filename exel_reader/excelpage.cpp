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
        std::unique_ptr<QAxObject> first_cell (m_table->querySubObject (
            "Cells(QVariant&,QVariant&)",
            QVariant (1),
            QVariant (1)
        ));
        std::unique_ptr<QAxObject> second_cell (m_table->querySubObject (
            "Cells(QVariant&,QVariant&)",
            QVariant (m_cache[0].toList ().size () + 1),
            QVariant (m_cache.size ())
        ));
        std::unique_ptr<QAxObject> range (m_table->querySubObject (
            "Range(const QVariant&,const QVariant&)",
            first_cell->asVariant (),
            second_cell->asVariant ()
        ));

        range->setProperty ("Value", QVariant (m_cache));
    }
    catch (...)
    {}
}

bool ExcelPage::set_x_axis_type(AxisType type)
{
    return true;
}

bool ExcelPage::save_data(const std::string& name, const std::vector<double>& data)
{
    if (m_cache.size () < data.size ())
    {
        for (size_t i = 0; i <= data.size (); ++i)
            m_cache.push_back(QList<QVariant> ());
    }

    m_cache[0].toList () << name.c_str ();
    QList<QVariant>::iterator iter = ++m_cache.begin ();
    for (size_t i = 0; i < data.size (); ++i)
    {
        (*iter++).toList () << data[i];
    }

    return true;
}

ExcelPage::ExcelPage(std::shared_ptr<ExcelFile> file, QAxObject* table) :
    m_file (file),
    m_table (table)
{}
