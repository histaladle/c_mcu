TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    dynarray.c \
    mastermodbus.c \
    modbusmaster.c \
    ringmemory.c

HEADERS += \
    dynarray.h \
    mastermodbus.h \
    modbusmaster.h \
    ringmemory.h \
    objectpool.h
