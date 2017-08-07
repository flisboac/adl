# -----------------------------------------------------------------------------
# CMake project wrapper Makefile ----------------------------------------------
# -----------------------------------------------------------------------------

SHELL := /bin/bash
RM	:= rm -rf
MKDIR := mkdir -p
BUILD_DIR := ./build
BUILD_DIRS := $(BUILD_DIR) cmake-build-* bin
SRC_DIR := ..

IDEA_FILES := .idea
ECLIPSE_FILES := .project .cproject .settings
KDEVELOP_FILES := *.kdev4 .kdev4

all: $(BUILD_DIR)/Makefile
	$(MAKE) -C $(BUILD_DIR)

clean: $(BUILD_DIR)/Makefile
	$(MAKE) -C $(BUILD_DIR) clean

docs: $(BUILD_DIR)/Makefile
	$(MAKE) -C $(BUILD_DIR) adl-doc

distclean:
	$(RM) $(BUILD_DIRS)

$(BUILD_DIR)/Makefile: CMakeLists.txt
	$(MKDIR) $(BUILD_DIR) > /dev/null
	cd $(BUILD_DIR) > /dev/null 2>&1 && cmake $(SRC_DIR)

eclipse-clean:
	$(RM) $(ECLIPSE_FILES)

kdevelop-clean:
	$(RM) $(KDEVELOP_FILES)

idea-clean:
	$(RM) $(IDEA_FILES)

ide-clean: eclipse-clean kdevelop-clean idea-clean
