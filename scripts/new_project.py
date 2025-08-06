import sys
from pathlib import Path

def create_project(bitloop_root: Path, name: str = None):
    # If caller didn’t supply a name, ask for it here
    if not name:
        name = input("Enter new project name: ").strip()
        if not name:
            print("Project name cannot be empty.", file=sys.stderr)
            sys.exit(1)

    template_dir = bitloop_root / "framework" / "templates" / "default"
    target_dir   = Path.cwd() / name

    print(f"Creating new project '{name}' at {target_dir}")
