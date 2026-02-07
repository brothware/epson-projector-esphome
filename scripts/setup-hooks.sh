#!/bin/bash
set -e

echo "Installing pre-commit hooks..."
pip install pre-commit
pre-commit install
pre-commit install --hook-type commit-msg

echo "Git hooks installed successfully"
