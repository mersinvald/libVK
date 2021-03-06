# Copyright (c) 2016 Mike Lubinets (aka mersinvald)
# See LICENSE

TEMPLATE = lib
CONFIG += static
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt

TARGET = VK

QMAKE_CXX = ccache $${QMAKE_CXX}
#QMAKE_CXXFLAGS += -O0
#QMAKE_CXXFLAGS += -fsanitize=memory -fsanitize-memory-track-origins -fPIE -fno-omit-frame-pointer -g  -std=c++11   -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -O2
#QMAKE_LFLAGS += -fsanitize=memory -fsanitize-memory-track-origins -pie -fno-omit-frame-pointer -g


DEFINES += BACKWARD_HAS_BFD=1
DEFINES += BACKWARD_HAS_DW=1

LIBS += -lcurl -lssl -lcrypto -lssl -lcrypto -llber -lldap -lz
LIBS += -lbfd -ldw

INCLUDEPATH += ./include/
DEPENDPATH  += ./include/
INCLUDEPATH += ./third-party/

SOURCES += \
    third-party/jsoncpp.cpp \
    vkapi.cpp \
    to_string.cpp \
    init.cpp \
    vkexception.cpp \
    third-party/backward.cpp

HEADERS += \
    include/vkapi.hpp \
    include/types.hpp \
    include/string_utils.hpp \
    include/log.hpp


