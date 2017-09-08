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
#ifndef EXCELDOCUMENT_READER_H
#define EXCELDOCUMENT_READER_H


#include "document/documentreader.h"


class ExcelDocumentReader final : public iDocumentReader
{
public:
    static pDocumentReader create (const std::string& filename);

    virtual ~ExcelDocumentReader ();

    virtual AxisType get_x_axis_type () override;

    virtual size_t get_columns_number () override;
    virtual size_t get_rows_number () override;

    virtual void get_headers (std::vector<std::string>* const) override;
    virtual void get_data (size_t row, std::vector<double>* const) override;

private:
    explicit ExcelDocumentReader(const std::string& filename);
    ExcelDocumentReader(const ExcelDocumentReader&) = delete;

    bool load_data ();

private:
    struct PrivateData;
    PrivateData* data;
};


#endif // EXCELDOCUMENT_READER_H
