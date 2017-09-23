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


#include "excelsavefile.h"

#include <ActiveQt/qaxobject.h>
#include <ActiveQt/qaxbase.h>


ExcelSaveFile::ExcelSaveFile(const std::string& filename) :
    m_filename (filename)
{
    m_workbook.reset (m_workbooks->querySubObject ("Add"));
    if (!m_workbook)
        return;
    QObject::connect(m_workbook.get (), SIGNAL(exception (int, QString, QString, QString)),
        this, SLOT(saveLastError (int, QString, QString, QString)));
}

ExcelSaveFile::~ExcelSaveFile()
{
    if (m_workbook)
        m_workbook->dynamicCall ("SaveCopyAs(const QVariant&)", QVariant (QString (m_filename.c_str ())));
}
