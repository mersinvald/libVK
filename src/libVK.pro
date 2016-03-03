TEMPLATE = lib
CONFIG += static
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt

TARGET = VK

#QMAKE_CXX = ccache $${QMAKE_CXX}
QMAKE_CXXFLAGS += -O0
#QMAKE_CXXFLAGS += -fsanitize=memory -fsanitize-memory-track-origins -fPIE -fno-omit-frame-pointer -g -O2
#QMAKE_LFLAGS += -fsanitize=memory -fsanitize-memory-track-origins -pie -fno-omit-frame-pointer -g

LIBS += -lcurl -lssl -lcrypto -lssl -lcrypto -llber -lldap -lz

INCLUDEPATH += ./include/
DEPENDPATH  += ./include/
INCLUDEPATH += ./third-party/

SOURCES += \
    third-party/jsoncpp.cpp \
    vkapi.cpp \
    vkjson.cpp \
    to_string.cpp \
    init.cpp

HEADERS += \
    include/vkapi.hpp \
    include/vkjson.hpp \
    include/types.hpp \
    include/string_utils.hpp \
    include/log.hpp

