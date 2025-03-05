# Makefile for AmiPixel PlatformIO Project

PROJECT_ENV = T-Display-AMOLED # Default PlatformIO environment (adjust if needed)
PLATFORMIO_CMD = pio                 # Command for PlatformIO CLI (usually 'pio' or 'platformio')

# --- Targets ---

.PHONY: all build upload clean monitor

all: build

build:
	@echo "Building AmiPixel project for environment: $(PROJECT_ENV)"
	@$(PLATFORMIO_CMD) run -e $(PROJECT_ENV)

upload:
	@echo "Uploading AmiPixel to device (environment: $(PROJECT_ENV))"
	@$(PLATFORMIO_CMD) run -t upload -e $(PROJECT_ENV)

clean:
	@echo "Cleaning build artifacts (environment: $(PROJECT_ENV))"
	@$(PLATFORMIO_CMD) run -t clean -e $(PROJECT_ENV)

monitor:
	@echo "Starting Serial Monitor (environment: $(PROJECT_ENV))"
	@$(PLATFORMIO_CMD) device monitor -e $(PROJECT_ENV)

py-pio-install:
	@echo "Python install of platformio starting"
	python -m pip install -U platformio

help:
	@echo "Makefile for AmiPixel PlatformIO Project"
	@echo ""
	@echo "Targets:"
	@echo "  all            - Builds the project (default target)"
	@echo "  build          - Builds the project for the specified environment"
	@echo "  upload         - Uploads the firmware to the device"
	@echo "  clean          - Cleans build artifacts"
	@echo "  monitor        - Starts the Serial Monitor"
	@echo "  py-pio-install - Installs PlatformIO CLI using Python pip"
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
	@echo "  make clean PROJECT_ENV=esp32dev    # Clean for 'esp32dev' environment"
	@echo "  make monitor                # Start serial monitor"
	@echo "  make py-pio-install        # Show PlatformIO CLI install command (Python pip)"
	@echo "  make help                   # Show this help message"
