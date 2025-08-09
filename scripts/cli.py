#!/usr/bin/env python3
import sys
import argparse
import platform
import os
import subprocess
from pathlib import Path

script_dir = Path(__file__).resolve().parent
sys.path.insert(0, str(script_dir))

from new_project   import create_project
from bootstrap     import run_bootstrap

def main():
    parser = argparse.ArgumentParser(prog="bitloop")

    # "[bitloop] ..." parser
    subparsers = parser.add_subparsers(dest="command", required=True)

    # "bitloop [...]" subparsers
    new_parser			= subparsers.add_parser("new",			help="scaffold something")
    bootstrap_parser	= subparsers.add_parser("bootstrap",	help="install bootstrap dependencies (Linux only)")

    # bitloop new [...] parser
    new_sub = new_parser.add_subparsers(dest="entity", required=True)
    new_sub.add_parser("project", help="scaffold a new project")


    args = parser.parse_args()

    if args.command == "bootstrap":
        run_bootstrap()
        return

    if args.command == "new" and getattr(args, "entity", None) == "project":
        bitloop_root = script_dir.parent
        create_project(bitloop_root=bitloop_root)
        return

if __name__ == "__main__":
    main()
