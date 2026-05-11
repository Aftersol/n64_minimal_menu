#   menudemo - A simple menu demo for the N64
#
#   This file builds the menudemo ROM for the N64.
#
#   Copyright (c) 2026 Aftersol
#
#   Permission is hereby granted, free of charge, to any person obtaining a copy
#   of this software and associated documentation files (the "Software"), to deal
#   in the Software without restriction, including without limitation the rights
#   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#   copies of the Software, and to permit persons to whom the Software is
#   furnished to do so, subject to the following conditions:
#
#   The above copyright notice and this permission notice shall be included in all
#   copies or substantial portions of the Software.
#
#   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
#   SOFTWARE.
#

SOURCE_DIR=src
BUILD_DIR=build
include $(N64_INST)/include/n64.mk

all: menudemo.z64
.PHONY: all

ASSETS_DIR = assets
FILESYSTEM_DIR = filesystem

assets = $(ASSETS_DIR)/background.png $(ASSETS_DIR)/logo.png
assets += $(ASSETS_DIR)/bop.wav64 $(ASSETS_DIR)/bap.wav64
assets += $(ASSETS_DIR)/miafan2010_-_you_would_be_here.xm64

assets_conv = $(addprefix $(FILESYSTEM_DIR)/,$(notdir $(assets:%.png=%.sprite)))

AUDIOCONV_FLAGS ?=
MKSPRITE_FLAGS ?=

OBJS = $(BUILD_DIR)/menudemo.o

$(FILESYSTEM_DIR)/%.sprite: $(ASSETS_DIR)/%.png
	@mkdir -p $(FILESYSTEM_DIR)
	@mkdir -p $(dir $@)
	@echo "    [SPRITE] $@"
	@$(N64_MKSPRITE) $(MKSPRITE_FLAGS) -o $(FILESYSTEM_DIR) "$<"

$(FILESYSTEM_DIR)/%.wav64: $(ASSETS_DIR)/%.wav
	@mkdir -p $(dir $@)
	@echo "    [AUDIOCONV] $@"
	@$(N64_AUDIOCONV) $(AUDIOCONV_FLAGS) -o $(FILESYSTEM_DIR) "$<"

$(FILESYSTEM_DIR)/%.xm64: $(ASSETS_DIR)/%.xm
	@mkdir -p $(dir $@)
	@echo "    [AUDIOCONV] $@"
	@$(N64_AUDIOCONV) $(AUDIOCONV_FLAGS) -o $(FILESYSTEM_DIR) "$<"


menudemo.z64: N64_ROM_TITLE="menudemo"
menudemo.z64: $(BUILD_DIR)/menudemo.dfs

$(BUILD_DIR)/menudemo.elf: $(OBJS)
$(BUILD_DIR)/menudemo.dfs: $(assets_conv)
	@echo "	[DFS] $@"
	if [ ! -s "$<"]; then rm -f "$<"; fi
	$(N64_MKDFS) "$@" $(FILESYSTEM_DIR) >/dev/null

clean:
	rm -f $(BUILD_DIR)/* *.z64
.PHONY: clean

-include $(wildcard $(BUILD_DIR)/*.d)