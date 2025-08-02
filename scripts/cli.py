#!/usr/bin/env python3
import sys
import argparse
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

        script_path = _script_dir / "bootstrap-deps.sh"
        if not script_path.exists():
            print(f"Error: Bootstrap script not found at {script_path}", file=sys.stderr)
            sys.exit(1)

        try:
            subprocess.run(["chmod", "+x", str(script_path)], check=True)
            subprocess.run([str(script_path)], check=True)
        except subprocess.CalledProcessError as e:
            print(f"Error running bootstrap script: {e}", file=sys.stderr)
            sys.exit(e.returncode)
        return

    if args.command == "new" and getattr(args, "entity", None) == "project":
        bitloop_root = _script_dir.parent
        create_project(bitloop_root=bitloop_root)
        return

if __name__ == "__main__":
    main()
