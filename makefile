# Makefile for AmiPixel PlatformIO Project

# Compiler and flags
CC = g++
CFLAGS = -Wall -Wextra -std=c++17

PROJECT_ENV = T-Display-AMOLED       # Default PlatformIO environment (adjust if needed)
TEST_ENV = test
VIRTUAL_ENV = qemu_esp32
PLATFORMIO_CMD = pio                 # Command for PlatformIO CLI (usually 'pio' or 'platformio')

# --- Targets ---

.PHONY: all build upload clean clean-libs clean-all monitor py-pio-install deploy test compdb uploadfs deployfs quick generate-stick-figures

all: build

build:
	@echo "Building AmiPixel project for environment: $(PROJECT_ENV)"
	@$(PLATFORMIO_CMD) run -e $(PROJECT_ENV)

upload:
	@echo "Uploading AmiPixel to device (environment: $(PROJECT_ENV))"
	@$(PLATFORMIO_CMD) run -t upload -e $(PROJECT_ENV)

uploadfs:
	@echo "Uploading AmiPixel filesystem image (SPIFFS) to device (environment: $(PROJECT_ENV))"
	@$(PLATFORMIO_CMD) run -t uploadfs -e $(PROJECT_ENV)

deployfs: upload uploadfs
	@echo "Deploying AmiPixel firmware and filesystem (environment: $(PROJECT_ENV))"
	@echo "Firmware and SPIFFS filesystem uploaded."

clean:
	@echo "Cleaning build artifacts (environment: $(PROJECT_ENV))"
	@$(PLATFORMIO_CMD) run -t clean -e $(PROJECT_ENV)

clean-libs:
	@echo "Purging downloaded libraries (.pio/libdeps)"
	@$(PLATFORMIO_CMD) run -t clean -e $(PROJECT_ENV)
	@if exist .pio\libdeps rmdir /s /q .pio\libdeps
	@echo "Library cache cleared. Next build will re-download libraries."

clean-all: clean clean-libs
	@echo "Complete clean: build artifacts and libraries purged"

monitor:
	@echo "Starting Serial Monitor (environment: $(PROJECT_ENV))"
	@$(PLATFORMIO_CMD) device monitor -e $(PROJECT_ENV)

deploy: clean build upload monitor

# Quick build and upload for development with stick figure generation
quick: generate-stick-figures build upload
	@echo "Quick build and upload completed!"

# Generate stick figures using the Python script
generate-stick-figures:
	@echo "Generating stick figure animations..."
	@python scripts/stick_figure_generator.py

vbuild:
	@echo "Building AmiPixel project for environment: $(VIRTUAL_ENV)"
	@$(PLATFORMIO_CMD) run -e $(VIRTUAL_ENV)

test:
	@echo "Starting Tests (environment: $(TEST_ENV))"
	@$(PLATFORMIO_CMD) test -e $(TEST_ENV) -vvv

py-pio-install:
	@echo "Python install of platformio starting"
	python -m pip install -U platformio

format:
	@echo "Running clang-format on source files"
	clang-format -i src/*.cpp src/*.h

compdb:
	@echo "Creates JSON file to use with clang"
	@$(PLATFORMIO_CMD) run -t compiledb

tidy:
	@echo "Running clang-tidy on source files"
	@$(PLATFORMIO_CMD) check

help:
	@echo "Makefile for AmiPixel PlatformIO Project"
	@echo ""
	@echo "Targets:"
	@echo "  all            - Builds the project (default target)"
	@echo "  build          - Builds the project for the specified environment"
	@echo "  upload         - Uploads the firmware to the device"
	@echo "  uploadfs       - Uploads the filesystem (SPIFFS) to the device"
	@echo "  deployfs       - Uploads both firmware and filesystem"
	@echo "  clean          - Cleans build artifacts"
	@echo "  clean-libs     - Purges downloaded libraries (.pio/libdeps)"
	@echo "  clean-all      - Complete clean: build artifacts and libraries"
	@echo "  monitor        - Starts the Serial Monitor"
	@echo "  deploy         - Clean, build, upload, and monitor"
	@echo "  quick          - Generate stick figures, build, and upload"
	@echo "  test           - Run unit tests"
	@echo "  py-pio-install - Installs PlatformIO CLI using Python pip"
	@echo "  format         - Formats the source files using clang-format"
	@echo "  tidy           - Lints the source files using clang-tidy"
	@echo ""
	@echo "Variables:"
	@echo "  PROJECT_ENV    - PlatformIO environment (default: $(PROJECT_ENV))"
	@echo "                   Override: make build PROJECT_ENV=your_env"
	@echo "  PLATFORMIO_CMD  - PlatformIO CLI command (default: $(PLATFORMIO_CMD))"
	@echo "                   Override: make PLATFORMIO_CMD=platformio"
	@echo ""
	@echo "Usage Examples:"
	@echo "  make build                  # Build for default environment"
	@echo "  make upload                 # Upload for default environment"
	@echo "  make clean-libs             # Purge library cache and force re-download"
	@echo "  make clean-all              # Complete clean including libraries"
	@echo "  make clean PROJECT_ENV=esp32dev    # Clean for 'esp32dev' environment"
	@echo "  make monitor                # Start serial monitor"
	@echo "  make py-pio-install        # Show PlatformIO CLI install command (Python pip)"
	@echo "  make format                 # Format source files using clang-format"
	@echo "  make tidy                   # Lint source files using clang-tidy"
	@echo "  make help                   # Show this help message"
