# Z80 Emulator 
# https://github.com/Andy4495/emulator-8-bit
# MIT License
#
# 0.1  11/29/22  Andy4495  Initial Creation
# v0.1.0    02/11/23  Andy4495 Read for first "release"

SUBDIRS = src

.PHONY: subdirs $(SUBDIRS)

all: subdirs emulator

clean:
	$(MAKE) -C $(SUBDIRS) $@
	rm -f emulator

debug verbose:
	$(MAKE) -C $(SUBDIRS) $@
	cp src/emulator .

subdirs: $(SUBDIRS)
     
$(SUBDIRS):
	$(MAKE) -C $@

emulator: src/emulator
	cp src/emulator .
