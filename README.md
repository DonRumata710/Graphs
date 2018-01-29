# Graphs
Application for wavelet and spectral analysis<br>

## Structure of project
- 3rd_party/qwt - [QWT library](http://qwt.sourceforge.net/index.html) version 6.1.3
- document_system - library for loading and saving data to files
- exel_reader - library for work with excel files
- graphs - sources of application
- include - folder with include files of libraries
- math_system - library work with math algorithms

## Requirement
For compiling this project it is needed to have installed [QT](https://www.qt.io/). Currently version 5.6 is used.<br>

## Program description
Application work with xlsx, xls and csv files on Windows. Only csv files are supported on Linux<br>
In excel files data should be located on the first page.<br>
First column is X axis. All others are Y. Columns can have headers. If they haven't headers they will be numerated automatically.<br>
In excel files saved data will be located on the relevant pages. If saving to csv files were chosen each page will be saved to its own file.

## Author
DonRumata710 (donrumata710@yandex.com)<br>
