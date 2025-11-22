#!/bin/bash
set -euo pipefail

source ~/third-party/emsdk/emsdk_env.sh
emcc main.c -o renderer.js -sUSE_WEBGPU=1
