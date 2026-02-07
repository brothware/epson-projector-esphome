def test_valid_model_names(models_module):
    names = models_module.get_model_names()
    assert "generic" in names
    assert "eh-tw7400" in names
    assert "eb-u42" in names


def test_model_names_are_lowercase(models_module):
    for name in models_module.get_model_names():
        assert name == name.lower(), f"Model name {name} should be lowercase"
