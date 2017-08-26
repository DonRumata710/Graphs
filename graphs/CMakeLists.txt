
set(APPLICATION_NAME graphs)

set(UTIL_HEADERS
    abstractionmodel
    choiseexception
    curve
    grid
    resource
    row
    scaledraw
    waveletstep
)


set(CLASSES
    application
    correlation
    graphmodel
    graphpresenter
    helper
    logscale
    matrixrasterdata
    multygraph
    plotdata
    plotmanager
    rangemaster
    smoothing
    spectr
    spectrogrampresenter
    swaveletperiod
    tabpresenter
    threadscontrol
    wavelemodel
    wavelet
    waveletdata
    waveletfunction
    waveletproducer
    zoomer
)

set(SOURCE_FILES
    main
)

set(HEADERS) 
set(SOURCES) 

foreach(header ${HEADER_FILES}) 
    LIST(APPEND HEADERS ${PROJECT_SOURCE_DIR}/${APPLICATION_NAME}/${header}.h) 
endforeach() 

foreach(class ${CLASSES}) 
    LIST(APPEND HEADERS ${PROJECT_SOURCE_DIR}/${APPLICATION_NAME}/${class}.h) 
    LIST(APPEND SOURCES ${PROJECT_SOURCE_DIR}/${APPLICATION_NAME}/${class}.cpp) 
endforeach() 


foreach(source ${SOURCE_FILES}) 
    LIST(APPEND SOURCES ${PROJECT_SOURCE_DIR}/${APPLICATION_NAME}/${source}.cpp) 
endforeach() 

include_directories(${QWT_ROOT})
target_link_libraries(${APPLICATION_NAME} qwt)
