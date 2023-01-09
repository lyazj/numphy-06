#!/usr/bin/env -S make -f

include ../Makefile.conf

# CFLAGS := $(subst -O3,-Og,$(CFLAGS))
RUN += 1d-plot 9d-plot
OUT += 1d-plot.pdf 9d-plot.pdf

include ../Makefile.rule

1d-plot.run: 1d.out
9d-plot.run: 9d.out
