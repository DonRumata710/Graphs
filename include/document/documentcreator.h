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
#ifndef DOCUMENTCREATOR_H
#define DOCUMENTCREATOR_H


#include "documentreader.h"
#include "documentwriter.h"

#include <string>
#include <unordered_map>


class DocumentCreator
{
public:
    static void add_readable_extention (const std::string& extention, pDocumentReader (*instance_creator)(const std::string&));
    static void add_writable_extention (const std::string& extention, pDocumentWriter (*instance_creator)(const std::string&));

    static pDocumentReader get_document_reader (const std::string& filename);
    static pDocumentWriter get_document_writer (const std::string& filename);

private:
    static std::unordered_map<std::string, pDocumentReader (*)(const std::string&)> m_reader_creators;
    static std::unordered_map<std::string, pDocumentWriter (*)(const std::string&)> m_writer_creators;
};


#define ADD_DOCUMENT_READER(reader, extention)                                  \
    class reader ## _reg                                                        \
    {                                                                           \
    public:                                                                     \
        reader ## _reg ()                                                       \
        {                                                                       \
            DocumentCreator::add_readable_extention(extention, reader::create); \
        }                                                                       \
    } reader ## _reg_instance;


#define ADD_DOCUMENT_WRITER(writer, extention)                                  \
    class writer ## _reg                                                        \
    {                                                                           \
    public:                                                                     \
        writer ## _reg ()                                                       \
        {                                                                       \
            DocumentCreator::add_writable_extention(extention, writer::create); \
        }                                                                       \
    } writer ## _reg_instance;


#endif // DOCUMENTCREATOR_H
