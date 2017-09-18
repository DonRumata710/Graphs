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
#include "excelfile.h"

#include <QVariant>
#include <ActiveQt/qaxobject.h>
#include <ActiveQt/qaxbase.h>


struct ExcelDocumentReader::PrivateData
{
    PrivateData (const std::string& filename) : file (filename)
    {}

    ExcelFile file;
    size_t num_columns = 0;
    size_t num_points = 0;
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
    headers->reserve(data->num_columns);

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
    row->reserve(data->num_points - (data->have_headers ? 1 : 0));

    QList<QVariant>::iterator iter (data->cache.begin () + (data->have_headers ? 1 : 0));
    while (iter!= data->cache.end ())
        row->push_back ((iter++)->toList ().at (index).toDouble ());
}

ExcelDocumentReader::ExcelDocumentReader(const std::string& filename) :
    data (new PrivateData (filename))
{}

size_t ExcelDocumentReader::get_rows_number()
{
    if (!data->num_columns)
    {
        std::unique_ptr<QAxObject> cell (data->file.get_table ()->querySubObject ("Cells(QVariant,Columns.Count)", QVariant (1)));
        std::unique_ptr<QAxObject> numColumnsEx (cell->querySubObject ("End (xlToRight)"));
        data->num_columns = numColumnsEx->property ("Column").toInt ();
    }
    return data->num_columns;
}

size_t ExcelDocumentReader::get_points_number()
{
    if (!data->num_points)
    {
        std::unique_ptr<QAxObject> cell (data->file.get_table ()->querySubObject ("Cells(Rows.Count,QVariant)", QVariant (1)));
        std::unique_ptr<QAxObject> numRowsEx (cell->querySubObject ("End (xlDown)"));
        data->num_points = numRowsEx->property ("Row").toInt ();
    }
    return data->num_points;
}

bool ExcelDocumentReader::load_data ()
{
    QAxObject* stat_sheet (data->file.get_table ());
    QAxObject* top_left_cell (stat_sheet->querySubObject ("Cells(QVariant&,QVariant&)", QVariant (1), QVariant (1)));
    QAxObject* bottom_right_cell (stat_sheet->querySubObject ("Cells(QVariant&,QVariant&)", QVariant (get_points_number ()), QVariant (get_rows_number ())));
    QAxObject* range (stat_sheet->querySubObject (
        "Range(const QVariant&,const QVariant&)", top_left_cell->asVariant (), bottom_right_cell->asVariant ()
    ));
    range->setProperty ("NumberFormat", QVariant ("Double"));

    data->cache = range->property ("Value").toList ();

    data->have_headers = is_text (data->cache.at (0).toList ().at (1).toString ().toStdString ());
    return true;
}
