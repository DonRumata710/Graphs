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


#include "exceldocument.h"
#include "excelfile.h"


struct ExcelDocument::PrivateData
{
    PrivateData (const std::string& filename) : file (filename)
    {}

    ExcelFile file;
};


ExcelDocument::ExcelDocument(const std::string& filename) :
    data (new PrivateData (filename))
{}

ExcelDocument::~ExcelDocument()
{
    delete data;
}

iDocument::AxisType ExcelDocument::get_x_axis_type()
{
    return TYPE_NUM;
}

size_t ExcelDocument::get_columns_number()
{
    return 0;
}

size_t ExcelDocument::get_rows_number()
{
    return 0;
}

std::vector<std::string> ExcelDocument::get_headers()
{
    return std::vector<std::string> ();
}

double ExcelDocument::get_item(size_t row, size_t column)
{
    return 0.0;
}