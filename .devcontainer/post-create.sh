#!/bin/sh

set -e

cmake --preset debug

if [ -f .devcontainer/post-create-local.sh ]; then
  sh .devcontainer/post-create-local.sh
fi
