import sys
from pathlib import Path

import pytest

sys.path.insert(0, str(Path(__file__).parent.parent / "components"))


@pytest.fixture
def models():
    from epson_projector.models import PROJECTOR_MODELS

    return PROJECTOR_MODELS
