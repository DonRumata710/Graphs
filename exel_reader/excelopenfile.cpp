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


#include "excelopenfile.h"

#include <ActiveQt/qaxobject.h>
#include <ActiveQt/qaxbase.h>


ExcelOpenFile::ExcelOpenFile(const std::string& filename)
{
    m_workbook.reset (m_workbooks->querySubObject ("Open(const QVariant&)", QVariant (filename.c_str ())));
    if (!m_workbook)
        return;
    QObject::connect(m_workbook.get (), SIGNAL(exception (int, QString, QString, QString)),
        this, SLOT(saveLastError (int, QString, QString, QString)));

    m_sheets.reset (m_workbook->querySubObject ("Sheets"));
    if (!m_sheets)
        return;
    QObject::connect(m_sheets.get (), SIGNAL(exception (int, QString, QString, QString)),
        this, SLOT(saveLastError (int, QString, QString, QString)));

    m_sheet.reset (m_sheets->querySubObject ("Item(const QVariant&)", QVariant (1)));
    if (!m_sheet)
        return;
    QObject::connect(m_sheet.get (), SIGNAL(exception (int, QString, QString, QString)),
        this, SLOT(saveLastError (int, QString, QString, QString)));
}

QAxObject* ExcelOpenFile::get_table() const
{
    return m_sheet.get ();
}
