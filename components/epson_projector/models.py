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


PROJECTOR_MODELS: dict[str, ProjectorModel] = {
    "eh-tw7400": {
        "name": "Epson EH-TW7400",
        "category": "home_cinema",
        "sources": {
            "HDMI1": "30",
            "HDMI2": "A0",
            "Component": "14",
        },
        "features": {
            "power": True,
            "lamp_hours": True,
            "mute": True,
            "volume": True,
            "brightness": {"min": 0, "max": 255},
            "contrast": {"min": -32, "max": 32},
            "color_modes": ["dynamic", "cinema", "natural", "bright_cinema"],
            "keystone_vertical": True,
            "keystone_horizontal": True,
            "luminance": True,
        },
    },
    "eh-tw9400": {
        "name": "Epson EH-TW9400",
        "category": "home_cinema",
        "sources": {
            "HDMI1": "30",
            "HDMI2": "A0",
            "Component": "14",
        },
        "features": {
            "power": True,
            "lamp_hours": True,
            "mute": True,
            "volume": True,
            "brightness": {"min": 0, "max": 255},
            "contrast": {"min": -32, "max": 32},
            "color_modes": ["dynamic", "cinema", "natural", "bright_cinema", "3d_dynamic", "3d_cinema"],
            "keystone_vertical": True,
            "keystone_horizontal": True,
            "luminance": True,
        },
    },
    "eb-u42": {
        "name": "Epson EB-U42",
        "category": "business",
        "sources": {
            "Computer1": "10",
            "Computer2": "20",
            "HDMI1": "30",
            "Video": "41",
            "USB": "52",
            "LAN": "53",
        },
        "features": {
            "power": True,
            "lamp_hours": True,
            "mute": True,
            "volume": True,
            "brightness": {"min": 0, "max": 255},
            "contrast": {"min": -32, "max": 32},
            "color_modes": ["presentation", "text", "srgb", "blackboard"],
            "keystone_vertical": True,
            "keystone_horizontal": False,
        },
    },
    "eb-2250u": {
        "name": "Epson EB-2250U",
        "category": "business",
        "sources": {
            "Computer1": "10",
            "Computer2": "20",
            "HDMI1": "30",
            "HDMI2": "A0",
            "Video": "41",
            "USB": "52",
            "LAN": "53",
        },
        "features": {
            "power": True,
            "lamp_hours": True,
            "mute": True,
            "volume": True,
            "brightness": {"min": 0, "max": 255},
            "contrast": {"min": -32, "max": 32},
            "color_modes": ["dynamic", "presentation", "cinema", "srgb"],
            "keystone_vertical": True,
            "keystone_horizontal": True,
        },
    },
    "generic": {
        "name": "Generic ESC/VP21",
        "category": "generic",
        "sources": {
            "HDMI1": "30",
            "HDMI2": "A0",
            "Computer1": "10",
            "Computer2": "20",
            "Video": "41",
            "S-Video": "42",
            "USB": "52",
            "LAN": "53",
        },
        "features": {
            "power": True,
            "lamp_hours": True,
            "mute": True,
            "volume": True,
            "brightness": {"min": 0, "max": 255},
            "contrast": {"min": -32, "max": 32},
            "keystone_vertical": True,
            "keystone_horizontal": True,
        },
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
