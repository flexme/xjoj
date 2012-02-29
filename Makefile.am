AUTOMAKE_OPTIONS=foreign
SUBDIRS=lib client server
EXTRA_DIST=web new.sql oj.sql trigger_procedure.sql script

install-data-local:
	rm -rf $(prefix)/etc
	mkdir $(prefix)/etc
	cp client/client_config.ini $(prefix)/etc/client_config.ini
	cp server/server_config.ini $(prefix)/etc/server_config.ini
	rm -rf $(prefix)/client_log
	rm -rf $(prefix)/server_log
	mkdir $(prefix)/client_log
	mkdir $(prefix)/server_log
	$(mkinstalldirs) server/data $(prefix)/data
	$(mkinstalldirs) server/data/in $(prefix)/data/in
	$(mkinstalldirs) server/data/out $(prefix)/data/out
	$(mkinstalldirs) server/data/spj $(prefix)/data/spj
	$(mkinstalldirs) server/data/src $(prefix)/data/src
	chmod 777 $(prefix)/data/src
	$(mkinstalldirs) server/data/tmp $(prefix)/data/tmp
	(cd client && $(SHELL) makejava)
	cp client/JavaSandbox.jar $(prefix)/bin
	cp client/libsandbox.so $(prefix)/bin

