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
    color_modes: dict[str, str]
    aspect_ratios: dict[str, str]
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

SOURCES_HDMI_SINGLE = {
    "HDMI1": "30",
}

SOURCES_HDMI_COMPONENT = {
    **SOURCES_HDMI,
    "Component": "14",
}

SOURCES_HDMI_COMPONENT_VIDEO = {
    **SOURCES_HDMI_COMPONENT,
    "Video": "41",
    "S-Video": "42",
}

SOURCES_BUSINESS_BASIC = {
    "Computer1": "10",
    "HDMI1": "30",
    "Video": "41",
    "USB": "52",
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

COLOR_MODE_CODES = {
    "Dynamic": "06",
    "Cinema": "05",
    "Natural": "07",
    "Bright Cinema": "0C",
    "sRGB": "01",
    "Presentation": "04",
    "Text": "02",
    "Blackboard": "11",
    "Photo": "14",
    "Sports": "08",
    "Custom": "10",
    "3D Dynamic": "15",
    "3D Cinema": "17",
}

COLOR_MODES_HOME_CINEMA = {
    "Dynamic": "06",
    "Cinema": "05",
    "Natural": "07",
    "Bright Cinema": "0C",
}

COLOR_MODES_HOME_CINEMA_3D = {
    **COLOR_MODES_HOME_CINEMA,
    "3D Dynamic": "15",
    "3D Cinema": "17",
}

COLOR_MODES_BUSINESS = {
    "Presentation": "04",
    "Text": "02",
    "sRGB": "01",
    "Blackboard": "11",
}

COLOR_MODES_BUSINESS_ALT = {
    "Dynamic": "06",
    "Presentation": "04",
    "Cinema": "05",
    "sRGB": "01",
}

ASPECT_RATIO_CODES = {
    "Normal": "00",
    "4:3": "10",
    "16:9": "20",
    "Auto": "30",
    "Full": "40",
    "Zoom": "50",
    "Native": "60",
    "Wide": "70",
    "Anamorphic": "80",
}

ASPECT_RATIOS_HOME_CINEMA = {
    "Normal": "00",
    "Auto": "30",
    "Full": "40",
    "Zoom": "50",
    "Wide": "70",
    "Anamorphic": "80",
}

ASPECT_RATIOS_BUSINESS = {
    "Normal": "00",
    "4:3": "10",
    "16:9": "20",
    "Auto": "30",
    "Full": "40",
    "Zoom": "50",
}


def _home_cinema(
    name: str,
    sources: dict[str, str] = SOURCES_HDMI,
    color_modes: dict[str, str] = COLOR_MODES_HOME_CINEMA,
    aspect_ratios: dict[str, str] = ASPECT_RATIOS_HOME_CINEMA,
    has_3d: bool = False,
) -> ProjectorModel:
    modes = COLOR_MODES_HOME_CINEMA_3D if has_3d else color_modes
    return {
        "name": name,
        "category": "home_cinema",
        "sources": sources,
        "features": {
            **FEATURES_BASE,
            "color_modes": modes,
            "aspect_ratios": aspect_ratios,
            "luminance": True,
        },
    }


def _business(
    name: str,
    sources: dict[str, str] = SOURCES_BUSINESS,
    color_modes: dict[str, str] = COLOR_MODES_BUSINESS,
    aspect_ratios: dict[str, str] = ASPECT_RATIOS_BUSINESS,
    keystone_horizontal: bool = True,
) -> ProjectorModel:
    return {
        "name": name,
        "category": "business",
        "sources": sources,
        "features": {
            **FEATURES_BASE,
            "color_modes": color_modes,
            "aspect_ratios": aspect_ratios,
            "keystone_horizontal": keystone_horizontal,
        },
    }


def _models_from_list(
    prefix: str,
    model_numbers: list[str],
    factory,
    **kwargs,
) -> dict[str, ProjectorModel]:
    return {f"{prefix}-{num}".lower(): factory(f"Epson {prefix.upper()}-{num}", **kwargs) for num in model_numbers}


PROJECTOR_MODELS: dict[str, ProjectorModel] = {
    # ==========================================================================
    # EH-TW Series - Home Cinema (Entry Level)
    # ==========================================================================
    **_models_from_list("EH", ["TW490", "TW570"], _home_cinema, sources=SOURCES_HDMI_SINGLE),
    **_models_from_list(
        "EH",
        ["TW610", "TW650", "TW700", "TW710", "TW740", "TW750"],
        _home_cinema,
    ),
    # ==========================================================================
    # EH-TW5xxx Series - Home Cinema (Mid-Range)
    # ==========================================================================
    **_models_from_list(
        "EH",
        [
            "TW5000",
            "TW5100",
            "TW5200",
            "TW5210",
            "TW5300",
            "TW5350",
            "TW5400",
            "TW5600",
            "TW5650",
            "TW5700",
            "TW5705",
            "TW5720",
            "TW5725",
            "TW5820",
            "TW5825",
        ],
        _home_cinema,
    ),
    # ==========================================================================
    # EH-TW6xxx Series - Home Cinema (Mid-Range)
    # ==========================================================================
    **_models_from_list(
        "EH",
        [
            "TW6000",
            "TW6100",
            "TW6150",
            "TW6250",
            "TW6600",
            "TW6700",
            "TW6800",
        ],
        _home_cinema,
        has_3d=True,
    ),
    # ==========================================================================
    # EH-TW7xxx Series - Home Cinema (High-End)
    # ==========================================================================
    **_models_from_list(
        "EH",
        ["TW7000", "TW7100"],
        _home_cinema,
    ),
    **_models_from_list(
        "EH",
        ["TW7200", "TW7300", "TW7400"],
        _home_cinema,
        sources=SOURCES_HDMI_COMPONENT,
    ),
    # ==========================================================================
    # EH-TW8xxx Series - Home Cinema (High-End with 3D)
    # ==========================================================================
    **_models_from_list(
        "EH",
        ["TW8000", "TW8100", "TW8200", "TW8300", "TW8400"],
        _home_cinema,
        sources=SOURCES_HDMI_COMPONENT,
        has_3d=True,
    ),
    # ==========================================================================
    # EH-TW9xxx Series - Home Cinema (Flagship)
    # ==========================================================================
    **_models_from_list(
        "EH",
        ["TW9000", "TW9100", "TW9200", "TW9300", "TW9400"],
        _home_cinema,
        sources=SOURCES_HDMI_COMPONENT,
        has_3d=True,
    ),
    # ==========================================================================
    # EH-LS Series - Home Cinema Laser
    # ==========================================================================
    **_models_from_list(
        "EH",
        ["LS300", "LS500", "LS650", "LS800"],
        _home_cinema,
    ),
    **_models_from_list(
        "EH",
        ["LS9000", "LS10000", "LS10500", "LS11000", "LS12000"],
        _home_cinema,
    ),
    # ==========================================================================
    # EB-S Series - Business (SVGA)
    # ==========================================================================
    **_models_from_list(
        "EB",
        ["S04", "S05", "S06", "S31", "S41"],
        _business,
        sources=SOURCES_BUSINESS_BASIC,
        keystone_horizontal=False,
    ),
    # ==========================================================================
    # EB-X Series - Business (XGA)
    # ==========================================================================
    **_models_from_list(
        "EB",
        ["X05", "X06", "X31", "X41", "X49", "X50", "X51"],
        _business,
        sources=SOURCES_BUSINESS_BASIC,
    ),
    # ==========================================================================
    # EB-W Series - Business (WXGA)
    # ==========================================================================
    **_models_from_list(
        "EB",
        ["W05", "W06", "W31", "W41", "W42", "W49", "W50", "W51", "W52"],
        _business,
    ),
    # ==========================================================================
    # EB-U Series - Business (WUXGA)
    # ==========================================================================
    **_models_from_list(
        "EB",
        ["U32", "U42", "U50"],
        _business,
    ),
    # ==========================================================================
    # EB-E Series - Business (Entry)
    # ==========================================================================
    **_models_from_list(
        "EB",
        ["E01", "E10", "E20"],
        _business,
        sources=SOURCES_BUSINESS_BASIC,
        keystone_horizontal=False,
    ),
    # ==========================================================================
    # EB-FH Series - Business (Full HD)
    # ==========================================================================
    **_models_from_list(
        "EB",
        ["FH06", "FH52"],
        _business,
        sources=SOURCES_BUSINESS_DUAL_HDMI,
        color_modes=COLOR_MODES_BUSINESS_ALT,
    ),
    # ==========================================================================
    # EB-17xx/18xx Series - Business (Portable)
    # ==========================================================================
    **_models_from_list(
        "EB",
        ["1780W", "1781W", "1785W", "1795F"],
        _business,
        sources=SOURCES_BUSINESS_DUAL_HDMI,
    ),
    # ==========================================================================
    # EB-2xxx Series - Business (Installation)
    # ==========================================================================
    **_models_from_list(
        "EB",
        [
            "2040",
            "2042",
            "2065",
            "2140W",
            "2142W",
            "2155W",
            "2165W",
            "2245U",
            "2247U",
            "2250U",
            "2255U",
            "2265U",
        ],
        _business,
        sources=SOURCES_BUSINESS_DUAL_HDMI,
        color_modes=COLOR_MODES_BUSINESS_ALT,
    ),
    # ==========================================================================
    # EB-9xx Series - Business (Meeting Room)
    # ==========================================================================
    **_models_from_list(
        "EB",
        ["970", "972", "980W", "982W", "990U", "992F"],
        _business,
        sources=SOURCES_BUSINESS_DUAL_HDMI,
    ),
    # ==========================================================================
    # EB-L Series - Business Laser
    # ==========================================================================
    **_models_from_list(
        "EB",
        ["L200F", "L200W", "L210W", "L255F", "L260F", "L265F"],
        _business,
        sources=SOURCES_BUSINESS_DUAL_HDMI,
        color_modes=COLOR_MODES_BUSINESS_ALT,
    ),
    # ==========================================================================
    # Generic Fallback
    # ==========================================================================
    "generic": {
        "name": "Generic ESC/VP21",
        "category": "generic",
        "sources": SOURCES_ALL,
        "features": {
            **FEATURES_BASE,
            "color_modes": COLOR_MODE_CODES,
            "aspect_ratios": ASPECT_RATIO_CODES,
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


def get_color_modes_for_model(model_id: str) -> dict[str, str]:
    model = get_model(model_id)
    if model is None:
        return {}
    return model["features"].get("color_modes", {})


def get_aspect_ratios_for_model(model_id: str) -> dict[str, str]:
    model = get_model(model_id)
    if model is None:
        return {}
    return model["features"].get("aspect_ratios", {})
