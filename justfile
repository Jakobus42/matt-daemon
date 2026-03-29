default: (build)

configure preset="debug" *args="":
    cmake --preset {{preset}} {{args}}

build preset="debug" *args="": (configure preset)
    cmake --build --preset {{preset}} {{args}}

test preset="debug" *args="": (build preset)
    ctest --preset {{preset}} {{args}}

analyze: (build "analyze")

doc: (build "doc")

doc-serve: doc
    python -m http.server 8000 -d doc/build/html

package preset="release": (build preset)
    cd build/{{preset}} && cpack

clean preset="debug":
    rm -rf build/{{preset}}

fclean:
    rm -rf build doc/build
