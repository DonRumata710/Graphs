/////////////////////////////////////////////////////////////////////
//
// Graphs
//
// Copyright (c) 2016-2018 DonRumata710
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
#include "excelpage.h"

#include <QVariant>
#include <ActiveQt/qaxobject.h>
#include <ActiveQt/qaxbase.h>


struct ExcelDocumentReader::PrivateData
{
    std::shared_ptr<ExcelFile> file;

    PrivateData (const std::string& filename) :
        file (std::make_shared<ExcelFile> ())
    {
        file->open_file (filename);
    }
};


pDocumentReader ExcelDocumentReader::create(const std::string& filename)
{
    return pDocumentReader (new ExcelDocumentReader (filename));
}

ExcelDocumentReader::~ExcelDocumentReader ()
{
    if (data)
        delete data;
    data = nullptr;
}

pPage ExcelDocumentReader::get_page(const std::string& name) const
{
    return std::shared_ptr<ExcelPage> (new ExcelPage (data->file, data->file->open_page (name)));
}

pPage ExcelDocumentReader::get_page(size_t index) const
{
    return std::shared_ptr<ExcelPage> (new ExcelPage (data->file, data->file->open_page (index)));
}

ExcelDocumentReader::ExcelDocumentReader(const std::string& filename) :
    data (new PrivateData (filename))
{}
