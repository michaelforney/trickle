SOURCES += main.cpp \
	filemodel.cpp \
	directoryitem.cpp \
	filemodelitem.cpp \
	fileitem.cpp \
	trackerinfo.cpp \
	trackermodel.cpp \
	trackeritem.cpp \
	trickle.cpp \
	torrentitem.cpp \
	log.cpp \
	updatetimer.cpp \
	torrentwidget.cpp \
	xmlrpc.cpp \
	torrentmodel.cpp \
	torrentview.cpp \
	torrentinfo.cpp \
	xmlrpcrequest.cpp \
	fileinfo.cpp \
	fileview.cpp \
	settingsdialog.cpp \
	settingspage.cpp \
	serverpage.cpp \
	settingsview.cpp \
	settingsmodel.cpp \
	servermodel.cpp \
	server.cpp \
	torrentsortmodel.cpp \
	bytesize.cpp \
	filedelegate.cpp \
	torrentdelegate.cpp \
	selectedtorrent.cpp \
	settingsdelegate.cpp \
	generalinfo.cpp \
	serverinfo.cpp \
	serverstatus.cpp \
	serveredit.cpp \
	generalpage.cpp

TEMPLATE = app
CONFIG += warn_on \
	  thread \
          qt
TARGET = ../bin/trickle

HEADERS += filemodel.h \
 directoryitem.h \
 filemodelitem.h \
 fileitem.h \
 trackerinfo.h \
 trackermodel.h \
 trackeritem.h \
 trickle.h \
 torrentitem.h \
 log.h \
 updatetimer.h \
 torrentwidget.h \
 xmlrpc.h \
 torrentmodel.h \
 torrentview.h \
 torrentinfo.h \
 xmlrpcrequest.h \
 fileinfo.h \
 fileview.h \
 settingsdialog.h \
 settingspage.h \
 serverpage.h \
 settingsview.h \
 settingsmodel.h \
 servermodel.h \
 server.h \
 torrentsortmodel.h \
 bytesize.h \
 filedelegate.h \
 torrentdelegate.h \
 selectedtorrent.h \
 settingsdelegate.h \
 generalinfo.h \
 serverinfo.h \
 serverstatus.h \
	serveredit.h \
	generalpage.h

FORMS += trackerinfo.ui \
	torrentinfo.ui \
	settingsdialog.ui \
	serveredit.ui \
	serverpage.ui

QT += network \
 xml

RESOURCES -= ftorrent.qrc

RESOURCES += trickle.qrc

