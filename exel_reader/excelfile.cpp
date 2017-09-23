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


#include "excelfile.h"
#include "excelpage.h"

#include <ActiveQt/qaxobject.h>
#include <ActiveQt/qaxbase.h>
#include <QDebug>


ExcelFile::~ExcelFile()
{
    if (!m_excel)
        return;

    m_excel->setProperty ("DisplayAlerts", QVariant ("False"));

    if (m_workbook)
        m_workbook->dynamicCall ("Close()");

    m_excel->dynamicCall ("Quit()");
}

std::unique_ptr<QAxObject> ExcelFile::create_page(const std::string& name)
{
    if (!m_workbook)
        return nullptr;

    QAxObject* new_stat_sheets (nullptr);

    try
    {
        std::unique_ptr<QAxObject> sheet_to_copy (m_workbook->querySubObject ("Worksheets(const QVariant&)", 1));
        if (sheet_to_copy)
            sheet_to_copy->dynamicCall ("Copy(const QVariant&)", sheet_to_copy->asVariant ());

        new_stat_sheets = m_workbook->querySubObject ("Worksheets(const QVariant&)", 1);
        if (new_stat_sheets)
            new_stat_sheets->setProperty ("Name", QString(name.c_str ()));
    }
    catch(...)
    {}

    return std::unique_ptr<QAxObject> (new_stat_sheets);
}

void ExcelFile::saveLastError(int err_code, QString source, QString description, QString help)
{
    qDebug() << err_code << source << description << help;
}

ExcelFile::ExcelFile() :
    m_excel     (std::make_unique<QAxObject> ("Excel.Application", this)),
    m_workbooks (m_excel ? m_excel->querySubObject ("Workbooks") : nullptr)
{
    QObject::connect(m_excel.get (), SIGNAL(exception (int, QString, QString, QString)),
        this, SLOT(saveLastError (int, QString, QString, QString)));
    QObject::connect(m_workbooks.get (), SIGNAL(exception (int, QString, QString, QString)),
        this, SLOT(saveLastError (int, QString, QString, QString)));
}
