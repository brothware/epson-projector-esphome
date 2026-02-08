def test_generic_model_exists(models):
    assert "generic" in models


def test_all_models_have_required_fields(models):
    required_fields = ["name", "category", "sources", "features"]
    for model_id, model in models.items():
        for field in required_fields:
            assert field in model, f"Model {model_id} missing field: {field}"


def test_all_models_have_power_feature(models):
    for model_id, model in models.items():
        features = model["features"]
        assert features.get("power") is True, f"Model {model_id} missing power feature"


def test_all_models_have_lamp_hours_feature(models):
    for model_id, model in models.items():
        features = model["features"]
        assert features.get("lamp_hours") is True, f"Model {model_id} missing lamp_hours feature"


def test_all_models_have_at_least_one_source(models):
    for model_id, model in models.items():
        sources = model["sources"]
        assert len(sources) > 0, f"Model {model_id} has no sources"


def test_source_codes_are_strings(models):
    for model_id, model in models.items():
        for source_name, source_code in model["sources"].items():
            assert isinstance(source_code, str), f"Model {model_id} source {source_name} code is not a string"


def test_get_model_returns_correct_model(models_module):
    model = models_module.get_model("generic")
    assert model is not None
    assert model["name"] == "Generic ESC/VP21"


def test_get_model_is_case_insensitive(models_module):
    model1 = models_module.get_model("generic")
    model2 = models_module.get_model("GENERIC")
    model3 = models_module.get_model("Generic")

    assert model1 == model2 == model3


def test_get_model_returns_none_for_unknown(models_module):
    model = models_module.get_model("unknown-model-xyz")
    assert model is None


def test_get_sources_for_model(models_module):
    sources = models_module.get_sources_for_model("generic")
    assert "HDMI1" in sources
    assert sources["HDMI1"] == "30"


def test_has_feature(models_module):
    assert models_module.has_feature("generic", "power") is True
    assert models_module.has_feature("generic", "nonexistent_feature") is False


def test_home_cinema_model_has_hdmi(models_module):
    sources = models_module.get_sources_for_model("eh-tw7400")
    assert "HDMI1" in sources
    assert "HDMI2" in sources


def test_business_model_has_computer_inputs(models_module):
    sources = models_module.get_sources_for_model("eb-u42")
    assert "Computer1" in sources


def test_eh_tw7100_model(models_module):
    model = models_module.get_model("eh-tw7100")
    assert model is not None
    assert model["name"] == "Epson EH-TW7100"
    assert model["category"] == "home_cinema"
    assert "HDMI1" in model["sources"]
    assert "HDMI2" in model["sources"]
    assert model["features"]["power"] is True
    assert model["features"]["luminance"]
