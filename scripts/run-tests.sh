#!/bin/bash
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
VENV_DIR="$PROJECT_ROOT/.venv"

if [ ! -d "$VENV_DIR" ]; then
    echo "Virtual environment not found. Run ./scripts/setup-hooks.sh first."
    exit 1
fi

source "$VENV_DIR/bin/activate"

echo "=== Running Python tests ==="
cd "$PROJECT_ROOT"
pytest tests/ -v --ignore=tests/cpp

echo ""
echo "=== Running C++ tests ==="
cd "$PROJECT_ROOT/tests/cpp"
mkdir -p build
cd build
cmake ..
make
ctest --output-on-failure

echo ""
echo "=== All tests passed ==="
