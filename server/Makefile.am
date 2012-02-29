bin_PROGRAMS=server
server_SOURCES=config.cpp judgeThread.cpp mysqlManager.cpp server.cpp taskProducer.cpp clientInfo.cpp clientInfo.h config.h judgeThread.h protocal.h connectmysql.h mysqlManager.h taskProducer.h
server_LDADD=$(top_srcdir)/lib/libxjoj.a
server_LDFLAGS=-D_GNU_SOURCE
INCLUDES=-I$(top_srcdir)/lib $(MYSQL_CFLAGS)
DEFS+=-D_GNU_SOURCE
LIBS+=-lpthread -lz `mysql_config --libs`
EXTRA_DIST=server_config.ini server_log

