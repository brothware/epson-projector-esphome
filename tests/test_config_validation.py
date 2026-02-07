import pytest

pytest.importorskip("esphome")

import esphome.config_validation as cv


def test_valid_model_names():
    from epson_projector.models import get_model_names

    names = get_model_names()
    assert "generic" in names
    assert "eh-tw7400" in names
    assert "eb-u42" in names


def test_model_names_are_lowercase():
    from epson_projector.models import get_model_names

    for name in get_model_names():
        assert name == name.lower(), f"Model name {name} should be lowercase"
