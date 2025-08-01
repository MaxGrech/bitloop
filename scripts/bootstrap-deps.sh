#!/usr/bin/env bash
set -euo pipefail
PKGS=(
  git cmake ninja-build build-essential curl zip unzip tar pkg-config
  autoconf automake autoconf-archive libltdl-dev
  libx11-dev libxft-dev libxext-dev
  libwayland-dev libxkbcommon-dev libegl1-mesa-dev wayland-protocols libdecor-0-dev
  libibus-1.0-dev wayland-protocols libdecor-0-dev libibus-1.0-dev
)
sudo apt-get update
sudo apt-get install -y "${PKGS[@]}"