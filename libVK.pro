TEMPLATE = lib
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt

#QMAKE_CXX = ccache $${QMAKE_CXX}
QMAKE_CXXFLAGS += -O0
#QMAKE_CXXFLAGS += -fsanitize=memory -fsanitize-memory-track-origins -fPIE -fno-omit-frame-pointer -g -O2
#QMAKE_LFLAGS += -fsanitize=memory -fsanitize-memory-track-origins -pie -fno-omit-frame-pointer -g

LIBS += -lcurl -lssl -lcrypto -lssl -lcrypto -llber -lldap -lz

INCLUDEPATH += ./include/
DEPENDPATH  += ./include/

SOURCES += \
    vkapi.cpp \
    include/jsoncpp/jsoncpp.cpp \
    log.cpp \
    vkjson.cpp \
    to_string.cpp

HEADERS += \
    vkapi.hpp \
    log.hpp \
    vkjson.hpp \
    types.hpp \
    string_utils.hpp
