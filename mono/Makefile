MONO_ROOT_DIR=.
MONO_BUILD_DIR=$(MONO_ROOT_DIR)/out
MONO_TEX_DIR=$(MONO_ROOT_DIR)/tex
MONO_IMG_DIR=$(MONO_ROOT_DIR)/img

RM=rm -f
RMDIR=rm -rf
LATEX_BUILD=latexmk -pdf -dvi-
LATEX_CLEAN=latexmk -c
LATEX_DISTCLEAN=latexmk -C

STRAY_FILES=mono.bbl mono.brf

all: build

build:
	pushd . && cd "$(MONO_ROOT_DIR)" && $(LATEX_BUILD) && popd

clean:
	pushd . && cd "$(MONO_ROOT_DIR)" && $(LATEX_CLEAN) && $(RM) $(STRAY_FILES) && popd
	pushd . && cd "$(MONO_TEX_DIR)" && $(LATEX_CLEAN) && popd

distclean:
	pushd . && cd "$(MONO_ROOT_DIR)" && $(LATEX_DISTCLEAN) && popd
	pushd . && cd "$(MONO_TEX_DIR)" && $(LATEX_DISTCLEAN) && popd
	$(RMDIR) "$(MONO_BUILD_DIR)"

