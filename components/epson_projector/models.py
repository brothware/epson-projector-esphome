from typing import TypedDict


class FeatureRange(TypedDict, total=False):
    min: int
    max: int


class Features(TypedDict, total=False):
    power: bool
    lamp_hours: bool
    mute: bool
    volume: bool
    brightness: FeatureRange
    contrast: FeatureRange
    color_modes: list[str]
    keystone_vertical: bool
    keystone_horizontal: bool
    luminance: bool


class ProjectorModel(TypedDict):
    name: str
    category: str
    sources: dict[str, str]
    features: Features


SOURCES_HDMI = {
    "HDMI1": "30",
    "HDMI2": "A0",
}

SOURCES_HDMI_COMPONENT = {
    **SOURCES_HDMI,
    "Component": "14",
}

SOURCES_BUSINESS = {
    "Computer1": "10",
    "Computer2": "20",
    "HDMI1": "30",
    "Video": "41",
    "USB": "52",
    "LAN": "53",
}

SOURCES_BUSINESS_DUAL_HDMI = {
    **SOURCES_BUSINESS,
    "HDMI2": "A0",
}

SOURCES_ALL = {
    "HDMI1": "30",
    "HDMI2": "A0",
    "Computer1": "10",
    "Computer2": "20",
    "Video": "41",
    "S-Video": "42",
    "USB": "52",
    "LAN": "53",
}

FEATURES_BASE: Features = {
    "power": True,
    "lamp_hours": True,
    "mute": True,
    "volume": True,
    "brightness": {"min": 0, "max": 255},
    "contrast": {"min": -32, "max": 32},
    "keystone_vertical": True,
    "keystone_horizontal": True,
}

COLOR_MODES_HOME_CINEMA = ["dynamic", "cinema", "natural", "bright_cinema"]
COLOR_MODES_HOME_CINEMA_3D = [*COLOR_MODES_HOME_CINEMA, "3d_dynamic", "3d_cinema"]
COLOR_MODES_BUSINESS = ["presentation", "text", "srgb", "blackboard"]
COLOR_MODES_BUSINESS_ALT = ["dynamic", "presentation", "cinema", "srgb"]


def _home_cinema(
    name: str,
    sources: dict[str, str] = SOURCES_HDMI,
    color_modes: list[str] = COLOR_MODES_HOME_CINEMA,
) -> ProjectorModel:
    return {
        "name": name,
        "category": "home_cinema",
        "sources": sources,
        "features": {
            **FEATURES_BASE,
            "color_modes": color_modes,
            "luminance": True,
        },
    }


def _business(
    name: str,
    sources: dict[str, str] = SOURCES_BUSINESS,
    color_modes: list[str] = COLOR_MODES_BUSINESS,
    keystone_horizontal: bool = True,
) -> ProjectorModel:
    return {
        "name": name,
        "category": "business",
        "sources": sources,
        "features": {
            **FEATURES_BASE,
            "color_modes": color_modes,
            "keystone_horizontal": keystone_horizontal,
        },
    }


PROJECTOR_MODELS: dict[str, ProjectorModel] = {
    "eh-tw5700": _home_cinema("Epson EH-TW5700"),
    "eh-tw5820": _home_cinema("Epson EH-TW5820"),
    "eh-tw7000": _home_cinema("Epson EH-TW7000"),
    "eh-tw7100": _home_cinema("Epson EH-TW7100"),
    "eh-tw7400": _home_cinema("Epson EH-TW7400", sources=SOURCES_HDMI_COMPONENT),
    "eh-tw9400": _home_cinema(
        "Epson EH-TW9400",
        sources=SOURCES_HDMI_COMPONENT,
        color_modes=COLOR_MODES_HOME_CINEMA_3D,
    ),
    "eh-ls12000": _home_cinema("Epson EH-LS12000"),
    "eb-u42": _business("Epson EB-U42", keystone_horizontal=False),
    "eb-2250u": _business(
        "Epson EB-2250U",
        sources=SOURCES_BUSINESS_DUAL_HDMI,
        color_modes=COLOR_MODES_BUSINESS_ALT,
    ),
    "generic": {
        "name": "Generic ESC/VP21",
        "category": "generic",
        "sources": SOURCES_ALL,
        "features": FEATURES_BASE,
    },
}


def get_model(model_id: str) -> ProjectorModel | None:
    return PROJECTOR_MODELS.get(model_id.lower())


def get_model_names() -> list[str]:
    return list(PROJECTOR_MODELS.keys())


def get_sources_for_model(model_id: str) -> dict[str, str]:
    model = get_model(model_id)
    if model is None:
        return {}
    return model["sources"]


def has_feature(model_id: str, feature: str) -> bool:
    model = get_model(model_id)
    if model is None:
        return False
    return bool(model["features"].get(feature))
