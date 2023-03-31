TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    mastermodbus.c \
    modbusmaster.c \
    ringmemory.c \
    vptrarray.c

HEADERS += \
    mastermodbus.h \
    modbusmaster.h \
    ringmemory.h \
    objectpool.h \
    vptrarray.h
