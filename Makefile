SUBDIRS = dotmat_driver \
	  pbt_driver

target :
	for DIRS in $(SUBDIRS); do \
		$(MAKE) -C $$DIRS; \
	done
