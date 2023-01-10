#!/usr/bin/env -S make -f

include ../Makefile.conf

# CFLAGS := $(subst -O3,-Og,$(CFLAGS))
RUN += q2-plot q3-plot q6-plot q10-plot
out += q2-plot.pdf q3-plot.pdf q6-plot.pdf q10-plot.pdf

include ../Makefile.rule

q2-plot.run: q2.out
q3-plot.run: q3.out
q6-plot.run: q6.out
q10-plot.run: q10.out
