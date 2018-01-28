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

#ifndef EXCELPAGE_H
#define EXCELPAGE_H


#include "document/page.h"
#include "excelfile.h"


class ExcelPage final : public iPage
{
    friend class ExcelDocumentWriter;
    friend class ExcelDocumentReader;

public:
    virtual ~ExcelPage ();

    virtual bool set_x_axis_type (AxisType type) override;
    virtual bool push_data_back (const std::string& name, const std::vector<double>& data) override;
    virtual bool push_data_back (const std::string& name, const double* data, size_t size) override;

    virtual AxisType get_x_axis_type () override;

    virtual bool get_headers(std::vector<std::string>* const) override;
    virtual bool get_data (size_t row, std::vector<double>* const) override;

private:
    ExcelPage (std::shared_ptr<ExcelFile> file, QAxObject* table);
    ExcelPage (std::shared_ptr<ExcelFile> file, std::unique_ptr<QAxObject>&& table);

    size_t get_rows_number ();
    size_t get_points_number ();
    bool load_data ();

private:
    std::shared_ptr<ExcelFile> m_file;
    std::unique_ptr<QAxObject> m_table;
    std::vector<QList<QVariant>> m_cache;

    size_t m_columns_num = 0;
    size_t m_points_num = 0;
    bool m_have_headers = false;
};


#endif // EXCELPAGE_H
