#!/usr/bin/env python3
import sys
import argparse
import platform
import os
import subprocess
from pathlib import Path

_script_dir = Path(__file__).resolve().parent
sys.path.insert(0, str(_script_dir))

from new_project import create_project

def main():
    parser = argparse.ArgumentParser(prog="bitloop")
    subparsers = parser.add_subparsers(dest="command", required=True)

    # `bitloop new project`
    new_parser = subparsers.add_parser("new", help="scaffold something")
    new_sub = new_parser.add_subparsers(dest="entity", required=True)
    new_sub.add_parser("project", help="scaffold a new project")

    # `bitloop bootstrap`
    subparsers.add_parser(
        "bootstrap",
        help="install bootstrap dependencies (Linux only)")

    args = parser.parse_args()

    if args.command == "bootstrap":
        # Only supported on Linux
        if platform.system() != "Linux":
            print("Error: 'bootstrap' is only supported on Linux.", file=sys.stderr)
            sys.exit(1)

        # Determine repo root
        bitloop_root = _script_dir.parent
        bashrc = Path.home() / ".bashrc"
        lines = [
            f"export BITLOOP_ROOT=\"{bitloop_root}\"",
            f"export PATH=\"$BITLOOP_ROOT:$PATH\""
        ]

        # Append to ~/.bashrc if not already present
        content = bashrc.read_text(encoding='utf-8') if bashrc.exists() else ""
        with bashrc.open('a', encoding='utf-8') as f:
            for line in lines:
                if line not in content:
                    f.write(f"\n{line}")

        # Make the wrapper script executable
        bitloop_sh = bitloop_root / "bitloop.sh"
        if bitloop_sh.exists():
            subprocess.run(["chmod", "+x", str(bitloop_sh)], check=True)
        else:
            print(f"Warning: bitloop.sh not found at {bitloop_sh}", file=sys.stderr)

        # Create a 'bitloop' symlink for easier invocation
        link_path = bitloop_root / "bitloop"
        if not link_path.exists() and bitloop_sh.exists():
            try:
                os.symlink(str(bitloop_sh), str(link_path))
            except OSError as e:
                print(f"Warning: could not create symlink {link_path}: {e}", file=sys.stderr)

        # Reload the shell config (may not affect current shell)
        try:
            subprocess.run(["bash", "-lc", "source ~/.bashrc"], check=True)
        except subprocess.CalledProcessError as e:
            print(f"Warning: failed to source ~/.bashrc: {e}", file=sys.stderr)

        # Install dependencies
        deps = [
            "git", "cmake", "ninja-build", "build-essential", "curl", "zip", "unzip", "tar", "pkg-config",
            "autoconf", "automake", "autoconf-archive", "libltdl-dev",
            "libx11-dev", "libxft-dev", "libxext-dev",
            "libwayland-dev", "libxkbcommon-dev", "libegl1-mesa-dev", "wayland-protocols", "libdecor-0-dev",
            "libibus-1.0-dev"
        ]
        try:
            subprocess.run(["sudo", "apt-get", "update"], check=True)
            subprocess.run(["sudo", "apt-get", "install", "-y"] + deps, check=True)
        except subprocess.CalledProcessError as e:
            print(f"Error installing dependencies: {e}", file=sys.stderr)
            sys.exit(e.returncode)

        print("Bootstrap completed: BITLOOP_ROOT set, PATH updated, bitloop.sh executable, 'bitloop' link created, and dependencies installed.")
        return


    if args.command == "new" and getattr(args, "entity", None) == "project":
        bitloop_root = _script_dir.parent
        create_project(bitloop_root=bitloop_root)
        return

if __name__ == "__main__":
    main()
