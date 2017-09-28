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

#pragma once
#ifndef EXCELFILE_H
#define EXCELFILE_H


#include <qobject.h>

#include <memory>
#include <string>


class QAxObject;
class ExcelPage;


class ExcelFile : public QObject
{
    Q_OBJECT

public:
    ExcelFile();
    ~ExcelFile();

    bool open_file (const std::string& filename);
    bool create_file(const std::string& filename);

    std::unique_ptr<QAxObject> open_page (uint32_t index) const;
    std::unique_ptr<QAxObject> open_page (const std::string& name) const;
    std::unique_ptr<QAxObject> create_page(const std::string& name);

    void save_as (const std::string& filename);

public slots:
    void save_last_error(int, QString, QString, QString);

protected:
    std::unique_ptr<QAxObject> m_excel;
    std::unique_ptr<QAxObject> m_workbooks;
    std::unique_ptr<QAxObject> m_workbook;
};

#endif // EXCELFILE_H
