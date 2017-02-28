# -----------------------------------------------------------------------------
# CMake project wrapper Makefile ----------------------------------------------
# -----------------------------------------------------------------------------

SHELL := /bin/bash
RM	:= rm -rf
MKDIR := mkdir -p
BUILD_DIR := ./build
SRC_DIR := ..

all: $(BUILD_DIR)/Makefile
	$(MAKE) -C $(BUILD_DIR)

$(BUILD_DIR)/Makefile:
	$(MKDIR) $(BUILD_DIR) > /dev/null
	cd $(BUILD_DIR) > /dev/null 2>&1 && cmake $(SRC_DIR)

distclean:
	$(RM) $(BUILD_DIR)

