# Copyright (c) 2016 Mike Lubinets (aka mersinvald)
# See LICENSE

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp
LIBS += -lcurl -lssl -lcrypto -lssl -lcrypto -llber -lldap -lz
LIBS += -ldl -lbfd -ldw

#QMAKE_CXX = ccache $${QMAKE_CXX}
QMAKE_CXXFLAGS += -O0
#QMAKE_CXXFLAGS += -fsanitize=memory -fsanitize-memory-track-origins -fPIE -fno-omit-frame-pointer -g -O2
#QMAKE_LFLAGS += -fsanitize=memory -fsanitize-memory-track-origins -pie -fno-omit-frame-pointer -g

# Add libVK
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../src/release/ -lVK
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../src/debug/ -lVK
else:unix: LIBS += -L$$OUT_PWD/../src/ -lVK

INCLUDEPATH += $$PWD/../include
DEPENDPATH += $$PWD/../include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../src/release/libVK.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../src/debug/libVK.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../src/release/VK.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../src/debug/VK.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../src/libVK.a
