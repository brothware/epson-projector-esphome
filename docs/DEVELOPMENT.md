# Development Guide

## Setup

```bash
# Clone the repository
git clone https://github.com/brothware/epson-projector-esphome.git
cd epson-projector-esphome

# Create virtual environment
python -m venv .venv
source .venv/bin/activate

# Install dependencies
pip install esphome pytest ruff mypy

# Install pre-commit hooks
pip install pre-commit
pre-commit install
```

## Running Tests

### All Tests

```bash
./scripts/run-tests.sh
```

### Python Tests

```bash
pytest tests/ -v --ignore=tests/cpp
```

### C++ Tests

```bash
cd tests/cpp
mkdir -p build && cd build
cmake ..
make -j$(nproc)
ctest --output-on-failure
```

Or run directly:

```bash
./tests/cpp/build/epson_tests
```

## Linting

```bash
# Run all linters
pre-commit run --all-files

# Individual linters
ruff check components/
ruff format components/
clang-format -i components/**/*.cpp components/**/*.h
```

## Building Test Configuration

```bash
esphome compile tests/esphome/test_config.yaml
```

## Project Structure

```
.
├── components/epson_projector/
│   ├── __init__.py          # Main component registration
│   ├── epson_projector.cpp  # Core hub implementation
│   ├── epson_projector.h    # Hub header
│   ├── response_parser.cpp  # Protocol response parsing
│   ├── command_queue.cpp    # Async command handling
│   ├── models.py            # Projector model definitions
│   ├── switch.py            # Switch platform
│   ├── sensor.py            # Sensor platform
│   ├── number.py            # Number platform
│   ├── select.py            # Select platform
│   └── ...
├── tests/
│   ├── cpp/                 # C++ unit tests (Google Test)
│   └── *.py                 # Python tests (pytest)
├── docs/                    # Documentation
└── .github/workflows/       # CI/CD
```

## Architecture

### Hub Pattern

The `EpsonProjector` class is the central hub that:
- Manages UART communication
- Maintains projector state
- Processes command queue
- Notifies child entities of state changes

### Entity Registration

Child entities (switches, sensors, etc.) register callbacks with the hub:

```cpp
void EpsonSwitch::setup() {
  parent_->add_on_state_callback([this]() { on_state_change(); });
  parent_->register_query(QueryType::POWER);
}
```

### Command Queue

Commands are queued and processed asynchronously:

1. Entity calls `parent_->set_power(true)`
2. Hub builds command string and enqueues
3. `loop()` sends command when ready
4. Response parsed in `handle_response()`
5. State callbacks notified

### Smart Polling

Only registered queries are sent during `update()`:

```cpp
void EpsonProjector::update() {
  for (const auto &info : QUERY_TABLE) {
    if (has_query(info.type)) {
      query(info.type);
    }
  }
}
```

## Adding New Features

### New Entity Type

1. Create `epson_<type>.h` and `.cpp`
2. Add Python platform file `<type>.py`
3. Register in `__init__.py`
4. Add QueryType if needed
5. Add tests

### New Projector Model

Edit `models.py`:

```python
"new-model": {
    "sources": ["HDMI1", "HDMI2", "USB"],
    "color_modes": ["Dynamic", "Cinema", "Natural"],
    "features": ["lens_shift", "4k"],
}
```

## Contributing

1. Fork the repository
2. Create feature branch
3. Write tests for new functionality
4. Ensure all tests pass
5. Run linters
6. Submit pull request

Branch protection requires:
- All CI checks passing
- One approval review
