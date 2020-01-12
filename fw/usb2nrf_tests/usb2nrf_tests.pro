TEMPLATE = app
CONFIG -= qt
CONFIG -= app_bundle
CONFIG += console

QMAKE_CXXFLAGS += -std=c++17


DEFINES += 'UNIT_TESTING=1'

isEmpty(BOOST_INCLUDE_DIR): BOOST_INCLUDE_DIR=$$(BOOST_INCLUDE_DIR)
!isEmpty(BOOST_INCLUDE_DIR): INCLUDEPATH *= $${BOOST_INCLUDE_DIR}

isEmpty(BOOST_INCLUDE_DIR): {
    message("BOOST_INCLUDE_DIR is not set, assuming Boost can be found automatically in your system")
}

SOURCES += \
    tests.cpp \
    "RFParser.c" \
    ../usb2nrf/functions.c \
    main.cpp \
    ../usb2nrf/messages.c

HEADERS += \
    "../usb2nrf/RF parser.h" \
    "../usb2nrf/UART parser.h" \
    ../usb2nrf/defines.h \
    ../usb2nrf/functions.h \
    ../usb2nrf/messages.h \
    ../usb2nrf/nrf24l01-mnemonics.h \
    ../usb2nrf/nrf24l01.h \
    tests.h

#OBJECTS = $$quote($$OBJECTS)
#OBJECTS += '"debug/RF parser.o"'
