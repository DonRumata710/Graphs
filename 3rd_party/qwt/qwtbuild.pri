################################################################
# Qwt Widget Library
# Copyright (C) 1997   Josef Wilgen
# Copyright (C) 2002   Uwe Rathmann
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the Qwt License, Version 1.0
################################################################

######################################################################
# qmake internal options
######################################################################

CONFIG           += qt     
CONFIG           += warn_on
CONFIG           += no_keywords
CONFIG           += silent

######################################################################
# release/debug mode
######################################################################

linux-g++ | linux-g++-64 {
    #CONFIG           += separate_debug_info
    #QMAKE_CXXFLAGS   *= -Wfloat-equal 
    #QMAKE_CXXFLAGS   *= -Wshadow 
    #QMAKE_CXXFLAGS   *= -Wpointer-arith 
    #QMAKE_CXXFLAGS   *= -Wconversion 
    #QMAKE_CXXFLAGS   *= -Wsign-compare 
    #QMAKE_CXXFLAGS   *= -Wsign-conversion 
    #QMAKE_CXXFLAGS   *= -Wlogical-op
    #QMAKE_CXXFLAGS   *= -Werror=format-security
    #QMAKE_CXXFLAGS   *= -std=c++11

    # when using the gold linker ( Qt < 4.8 ) - might be 
    # necessary on non linux systems too
    #QMAKE_LFLAGS += -lrt
}

######################################################################
# paths for building qwt
######################################################################

RCC_DIR = resources
