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


#include "exceldocumentreader.h"
#include "excelopenfile.h"

#include <QVariant>
#include <ActiveQt/qaxobject.h>
#include <ActiveQt/qaxbase.h>


struct ExcelDocumentReader::PrivateData
{
    PrivateData (const std::string& filename) : file (filename)
    {}

    ExcelOpenFile file;
    size_t columns_num = 0;
    size_t points_num = 0;
    bool have_headers = false;
    QList<QVariant> cache;
};


pDocumentReader ExcelDocumentReader::create(const std::string& filename)
{
    return pDocumentReader (new ExcelDocumentReader (filename));
}

ExcelDocumentReader::~ExcelDocumentReader()
{
    delete data;
}

AxisType ExcelDocumentReader::get_x_axis_type()
{
    std::unique_ptr<QAxObject> cell (data->file.get_table ()->querySubObject (
        "Cells(QVariant,QVariant)", QVariant (2), QVariant (1)
    ));
    return get_str_type (cell->property ("Value").toString ().toStdString ());
}

void ExcelDocumentReader::get_headers(std::vector<std::string>* const headers)
{
    if (data->cache.isEmpty ())
    {
        if (!load_data ())
            return;
    }

    headers->clear ();
    headers->reserve(data->columns_num);

    QList<QVariant> header_list (data->cache.front ().toList ());
    QList<QVariant>::iterator iter (header_list.begin ());
    while (iter != header_list.end ())
        headers->push_back ((iter++)->toString ().toStdString ());
}

void ExcelDocumentReader::get_data(size_t index, std::vector<double>* const row)
{
    if (data->cache.isEmpty ())
    {
        if (!load_data ())
            return;
    }

    if (index > data->cache.size ())
        return;

    row->clear ();
    row->reserve(data->points_num - (data->have_headers ? 1 : 0));

    QList<QVariant>::iterator iter (data->cache.begin () + (data->have_headers ? 1 : 0));
    while (iter!= data->cache.end ())
        row->push_back ((iter++)->toList ().at (index).toDouble ());
}

ExcelDocumentReader::ExcelDocumentReader(const std::string& filename) :
    data (new PrivateData (filename))
{}

size_t ExcelDocumentReader::get_rows_number()
{
    if (!data->columns_num)
    {
        std::unique_ptr<QAxObject> cell (data->file.get_table ()->querySubObject ("Cells(QVariant,Columns.Count)", QVariant (1)));
        if (!cell)
            return 0;
        std::unique_ptr<QAxObject> columns_num (cell->querySubObject ("End (xlToRight)"));
        if (!columns_num)
            return 0;
        data->columns_num = columns_num->property ("Column").toInt ();
    }
    return data->columns_num;
}

size_t ExcelDocumentReader::get_points_number()
{
    if (!data->points_num)
    {
        std::unique_ptr<QAxObject> cell (data->file.get_table ()->querySubObject ("Cells(Rows.Count,QVariant)", QVariant (1)));
        if (!cell)
            return 0;
        std::unique_ptr<QAxObject> numRowsEx (cell->querySubObject ("End (xlDown)"));
        if (!numRowsEx)
            return 0;
        data->points_num = numRowsEx->property ("Row").toInt ();
    }
    return data->points_num;
}

bool ExcelDocumentReader::load_data ()
{
    QAxObject* sheet (data->file.get_table ());

    if (!sheet)
        return false;

    std::unique_ptr<QAxObject> top_left_cell (sheet->querySubObject ("Cells(QVariant&,QVariant&)", QVariant (1), QVariant (1)));
    std::unique_ptr<QAxObject> bottom_right_cell (sheet->querySubObject ("Cells(QVariant&,QVariant&)", QVariant (get_points_number ()), QVariant (get_rows_number ())));

    if (!top_left_cell || !bottom_right_cell)
        return false;

    std::unique_ptr<QAxObject> range (sheet->querySubObject (
        "Range(const QVariant&,const QVariant&)", top_left_cell->asVariant (), bottom_right_cell->asVariant ()
    ));

    if (!range)
        return false;

    range->setProperty ("NumberFormat", QVariant ("Double"));

    data->cache = range->property ("Value").toList ();

    data->have_headers = is_text (data->cache.at (0).toList ().at (1).toString ().toStdString ());
    return true;
}
