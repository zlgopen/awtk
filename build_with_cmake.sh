#!/usr/bin/env bash
set -euo pipefail

BUILD_TYPE="Debug"
AWTK_DEBUG_BOOL="ON"

usage() {
  cat <<'EOF'
Usage: build_with_cmake.sh [Debug|Release] [-- extra cmake configure flags...]

  Debug    (default)  -DCMAKE_BUILD_TYPE=Debug   -DAWTK_DEBUG=ON
  Release             -DCMAKE_BUILD_TYPE=Release -DAWTK_DEBUG=OFF

After optional "--", remaining arguments are passed only to the configure step, e.g.:
  build_with_cmake.sh Release -- -DAWTK_SDL_VERSION=3

Any configure-style flag before "--" is also forwarded (e.g. -DAWTK_RUN_IDL_CODEGEN=ON).

Environment:
  INSTALL_PREFIX     cmake --install prefix (default: ../awtk-install)
  CMAKE_PREFIX_PATH  for find_package (default: $PWD/../awtk-install)
  JOBS               parallel compile jobs (default: CPU count)
  BUILD_DIR          out-of-tree build directory (default: build)

Multi-config generators (Visual Studio, Xcode): build and install always pass
--config Debug|Release matching your selection; single-config generators ignore it.
EOF
}

CMAKE_EXTRA=()
while [[ $# -gt 0 ]]; do
  case "$1" in
    -h | --help)
      usage
      exit 0
      ;;
    --)
      shift
      CMAKE_EXTRA+=("$@")
      break
      ;;
    [Rr]elease)
      BUILD_TYPE="Release"
      AWTK_DEBUG_BOOL="OFF"
      shift
      ;;
    [Dd]ebug)
      BUILD_TYPE="Debug"
      AWTK_DEBUG_BOOL="ON"
      shift
      ;;
    -*)
      CMAKE_EXTRA+=("$1")
      shift
      ;;
    *)
      echo "Unknown argument: $1" >&2
      echo "Try: $0 --help" >&2
      exit 1
      ;;
  esac
done

: "${INSTALL_PREFIX:=../awtk-install}"
: "${CMAKE_PREFIX_PATH:=${PWD}/../awtk-install}"
: "${BUILD_DIR:=build}"
export CMAKE_PREFIX_PATH

NPROC="$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)"
if [[ -n "${JOBS:-}" ]]; then
  NPROC="${JOBS}"
fi

echo "CMAKE_BUILD_TYPE=${BUILD_TYPE}  AWTK_DEBUG=${AWTK_DEBUG_BOOL}"
echo "BUILD_DIR=${BUILD_DIR}  INSTALL_PREFIX=${INSTALL_PREFIX}"
echo "CMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}"

# With set -u, "${CMAKE_EXTRA[@]}" on an empty array errors on bash 5+; use + guard.
cmake -B "${BUILD_DIR}" -S . \
  -DCMAKE_BUILD_TYPE="${BUILD_TYPE}" \
  -DAWTK_DEBUG="${AWTK_DEBUG_BOOL}" \
  "${CMAKE_EXTRA[@]+"${CMAKE_EXTRA[@]}"}"

cmake --build "${BUILD_DIR}" --config "${BUILD_TYPE}" --parallel "${NPROC}"

cmake --install "${BUILD_DIR}" --config "${BUILD_TYPE}" --prefix="${INSTALL_PREFIX}"
