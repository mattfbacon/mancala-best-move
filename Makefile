.PHONY: clean distclean debug release

BUILD_DIR := build
DIST_DIR := dist

TARGET := mancala

CMAKE := cmake
NINJA := ninja

CMAKE_FLAGS := \
	-DCMAKE_C_COMPILER=$(shell which clang) \
	-DCMAKE_CXX_COMPILER=$(shell which clang++) \
	-DCMAKE_LINKER=$(shell which lld) \
	--no-warn-unused-cli

export CMAKE_GENERATOR = Ninja Multi-Config

$(BUILD_DIR)/build-Debug.ninja $(BUILD_DIR)/build-Release.ninja &: CMakeLists.txt
	cmake $(CMAKE_FLAGS) -H. -B$(BUILD_DIR)

debug: $(BUILD_DIR)/build-Debug.ninja
	$(NINJA) -C $(dir $<) -f $(notdir $<)

release: $(BUILD_DIR)/build-Release.ninja
	$(NINJA) -C $(dir $<) -f $(notdir $<)

clean: $(BUILD_DIR)/build-Debug.ninja $(BUILD_DIR)/build-Release.ninja
	for prereq in $^; do \
		$(NINJA) -C $$(dirname $$prereq) -f $$(basename $$prereq) clean; \
	done

distclean: clean
	find $(BUILD_DIR) -mindepth 1 -delete

.DEFAULT_GOAL = release
