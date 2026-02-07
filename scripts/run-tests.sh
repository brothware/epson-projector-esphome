#!/bin/bash
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

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
