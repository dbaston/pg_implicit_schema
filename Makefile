
MODULE_big = implicit_schema
OBJS = implicit_schema.o
EXTENSION = implicit_schema
DATA = implicit_schema--0.1.sql
REGRESS = implicit_schema
EXTRA_CLEAN =

PG_CONFIG = pg_config

ifdef DEBUG
COPT			+= -O0
endif

PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)

