UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
# Linux Setup
else ifeq ($(UNAME), Windows)
# Windows Setup
else ifew ($(UNAME), MacOS)
# Mac Setup
endif
