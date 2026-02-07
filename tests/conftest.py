import importlib.util
from pathlib import Path

import pytest

COMPONENT_DIR = Path(__file__).parent.parent / "components" / "epson_projector"


def load_module_directly(name: str):
    spec = importlib.util.spec_from_file_location(name, COMPONENT_DIR / f"{name}.py")
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


@pytest.fixture
def models():
    module = load_module_directly("models")
    return module.PROJECTOR_MODELS


@pytest.fixture
def models_module():
    return load_module_directly("models")
