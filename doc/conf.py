project = "matt-daemon"

extensions = [
    "breathe",
    "myst_parser",
]

breathe_projects = {"matt-daemon": "build/doxygen/xml"}
breathe_default_project = "matt-daemon"

html_theme = "furo"
html_static_path = ["_static"]
html_theme_options = {
    "light_logo": "logo.png",
    "dark_logo": "logo.png",
}

exclude_patterns = [
    "doc/build",
    "build",
    "README.md",
    "**/README.md",
    "**/src",
    "**/test",
    "**/include",
    "**/example",
    "**/demo",
]
