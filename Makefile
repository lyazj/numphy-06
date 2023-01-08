#!/usr/bin/env -S make -f

DIR = $(shell ls -F | grep /$$)

all run clean:
	@for d in $(DIR); do $(MAKE) $@ -C $$d; done

show:
	@echo DIR = $(DIR)
	@for d in $(DIR); do $(MAKE) $@ -C $$d; done

.PHONY: all run clean show
