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


#include "graphmodel.h"


GraphModel* GraphModel::get_correlations (double begin, double end, unsigned step) const
{
    return new GraphModel (m_source.get_correlations (begin, end, step));
}

GraphModel* GraphModel::get_relative_sp (double begin, double end, unsigned step) const
{
    return new GraphModel (m_source.get_relative_sp (begin, end, step));
}

GraphModel* GraphModel::get_power () const
{
    return new GraphModel (m_source.get_power ());
}

void GraphModel::load_data(pDocumentReader document)
{
    set_type(document->get_x_axis_type ());
    m_source.load_data (document);
}

void GraphModel::save_data(pDocumentWriter document) const
{
}

/*
void GraphModel::save_data (QAxObject* workbook) const
{
    unique_ptr<QAxObject> sheetToCopy (workbook->querySubObject ("Worksheets(const QVariant&)", 1));
    sheetToCopy->dynamicCall ("Copy(const QVariant&)", sheetToCopy->asVariant ());
    unique_ptr<QAxObject> newSheet (workbook->querySubObject ("Worksheets(const QVariant&)", 1));
    newSheet->setProperty ("Name", QString(m_source.get_name ().c_str ()));
    m_source.save_data (newSheet.get ());
}

void GraphModel::load_data (QAxObject* workbook)
{
    //unique_ptr<QAxObject> mExcel (new QAxObject ("Excel.Application", this));
    //unique_ptr<QAxObject> workbooks (mExcel->querySubObject ("Workbooks"));
    //unique_ptr<QAxObject> workbook (workbooks->querySubObject ("Open(const QVariant&)", QVariant (filename)));

    unique_ptr<QAxObject> mSheets (workbook->querySubObject ("Sheets"));
    unique_ptr<QAxObject> StatSheet (mSheets->querySubObject ("Item(const QVariant&)", QVariant (1)));

    unique_ptr<QAxObject> cell (StatSheet->querySubObject ("Cells(QVariant,QVariant)", QVariant (2), QVariant (1)));
    set_type (get_str_type (cell->property ("Value").toString ().toStdString ()));

    m_source.set_data (StatSheet.get ());
}
*/

StringList GraphModel::get_headers() const { return m_source.get_headers (); }

string GraphModel::get_name () const { return m_source.get_name (); }
