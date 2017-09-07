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


AxisType get_str_type (const std::string& str)
{
    auto s (str.begin ());
    unsigned i (1);
    QChar sep = '\n';
    int lng[5] = { 0 };
    bool lettersLong (false);
    bool letters (false);
    if (!isdigit(*s)) return TYPE_NUM;
    ++s;
    //if (!(*s++).isDigit ())
    while (s != str.end ())
    {
        if (isdigit(*s)) i++;
        else
        {
            if (isalpha(*s))
            {
                if (letters && lettersLong)
                    return TYPE_NUM;
                else
                {
                    unsigned j (0);
                    while (isalpha(*s) && s++ != str.end ())
                        ++j;
                    if (j > 2 && !lettersLong)
                        lettersLong = TYPE_TIME;
                    else if (!letters)
                        letters = TYPE_TIME;
                    else
                        return TYPE_NUM;
                }
            }
            else if (sep == *s || *s == ',') sep = '\n';
            else if (sep == '\n') sep = *s;
            else return TYPE_NUM;
            if (i > 4) return TYPE_NUM;
            ++lng[i];
            i = 0;
        }
        ++s;
    }
    if (lng[4] < 2 && lng[2] != 0 &&
        ((lng[1] + lng[2] + lng[4] > 1 && lettersLong && lng[1] + lng[2] + lng[4] < 5) ||
        (lng[1] + lng[2] + lng[4] > 2 && !lettersLong && lng[1] + lng[2] + lng[4] < 6)))
        return TYPE_TIME;
    return TYPE_NUM;
}

bool is_text (QString str)
{
    return !(str.begin ()->isDigit () && (str.end () - 1)->isDigit ());
}


struct ExcelDocumentReader::PrivateData
{
    PrivateData (const std::string& filename) : file (filename)
    {}

    void load_data ()
    {
        if (!num_columns && !num_rows)
            return;

        QAxObject* top_left_cell (StatSheet->querySubObject ("Cells(QVariant&,QVariant&)", QVariant (1), QVariant (1)));
        QAxObject* bottom_right_cell (StatSheet->querySubObject ("Cells(QVariant&,QVariant&)", QVariant (num_rows), QVariant (num_columns)));
        QAxObject* range (StatSheet->querySubObject (
            "Range(const QVariant&,const QVariant&)", top_left_cell->asVariant (), bottom_right_cell->asVariant ()
        ));
        range->setProperty ("NumberFormat", QVariant ("Double"));

        cache = range->property ("Value").toList ();
    }

    ExcelFile file;
    size_t num_columns = 0;
    size_t num_rows = 0;
    QList<QVariant> cache;
};


pDocumentReader ExcelDocumentReader::create(const std::string& filename)
{
    return pDocumentReader (new ExcelDocumentReader (filename));
}

ExcelDocumentReader::ExcelDocumentReader(const std::string& filename) :
    data (new PrivateData (filename))
{}

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

size_t ExcelDocumentReader::get_columns_number()
{
    if (!data->numColumns)
    {
        std::unique_ptr<QAxObject> cell (StatSheet->querySubObject ("Cells(QVariant,Columns.Count)", QVariant (1)));
        std::unique_ptr<QAxObject> numColumnsEx = cell->querySubObject ("End (xlToRight)");
        data->numColumns = numColumnsEx->property ("Column").toInt ();
    }
    return data->numColumns;
}

size_t ExcelDocumentReader::get_rows_number()
{
    if (!data->num_rows)
    {
        std::unique_ptr<QAxObject> cell (StatSheet->querySubObject ("Cells(Rows.Count,QVariant)", QVariant (1)));
        std::unique_ptr<QAxObject> numRowsEx (cell->querySubObject ("End (xlDown)"));
        data->numRows = numRowsEx->property ("Row").toInt ();
    }
    return data->numRows;
}

std::vector<std::string> ExcelDocumentReader::get_headers()
{
    if (cache.isEmpty ())
        data->load_data ();

    return std::vector<std::string> ();
}

void ExcelDocumentReader::get_data(std::vector<double>* const data)
{
    for (unsigned row = 0; row < data->num_rows - (noHeader ? 0 : 1); ++row)
    {
        QList<QVariant> rowList (list[row].toList ());
        for (unsigned col = 0; col < numColumns; ++col)
            get_series ()[col][row] = rowList[col].toDouble ();
    }
}
