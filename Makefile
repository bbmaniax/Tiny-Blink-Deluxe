SKETCH ?= Tiny-Blink-Deluxe.ino
CORES ?= rp2040:rp2040 ATTinyCore:avr MicroCore:avr
LIBS ?=

BUILD_CONFIG ?= ./arduino-cli.yaml
BUILD_DIR ?= ./build

DEPLOY_FQBN ?= rp2040:rp2040:rpipico
DEPLOY_CMD ?= /mnt/c/Windows/System32/robocopy.exe
DEPLOY_DIR ?= D:/

TEMP_DIR ?= ./tmp

define build-sketch
	arduino-cli compile \
		--library ./src \
		--fqbn $(1) \
		--export-binaries \
		$(if $(filter-out undefined,$(origin DEBUG)),--build-property "build.extra_flags=-DDEBUG") \
		./$(2);
endef

.PHONY: all
all: clean install build

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(TEMP_DIR)
	find . -type f -exec chmod -x {} +

.PHONY: build
build: build/rpipico build/attiny85 build/attiny13

.PHONY: build/rpipico
build/rpipico:
	$(call build-sketch,rp2040:rp2040:rpipico,$(SKETCH))

.PHONY: build/attiny85
build/attiny85:
	$(call build-sketch,ATTinyCore:avr:attinyx5,$(SKETCH))

.PHONY: build/attiny13
build/attiny13:
	$(call build-sketch,MicroCore:avr:13,$(SKETCH))

.PHONY: install
install: core/install lib/install

.PHONY: core/install
core/install:
	@if [ ! -f ~/.arduino15/arduino-cli.yaml ]; then arduino-cli config init; fi
	arduino-cli --config-file $(BUILD_CONFIG) core update-index
	arduino-cli --config-file $(BUILD_CONFIG) core install $(CORES)

.PHONY: lib/install
lib/install:
# 	@if [ ! -f ~/.arduino15/arduino-cli.yaml ]; then arduino-cli config init; fi
# 	arduino-cli --config-file $(BUILD_CONFIG) lib update-index
# 	arduino-cli --config-file $(BUILD_CONFIG) lib install $(LIBS)

.PHONY: deploy
deploy:
	-$(DEPLOY_CMD) "$(subst /,\,$(BUILD_DIR)/$(subst :,.,$(DEPLOY_FQBN)))" "$(DEPLOY_DIR)" *.uf2
