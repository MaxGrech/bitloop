import sys
from pathlib import Path

def create_project(bitloop_root: Path, name: str = None):
    # If caller didn’t supply a name, ask for it here
    if not name:
        name = input("Enter new project name: ").strip()
        if not name:
            print("Project name cannot be empty.", file=sys.stderr)
            sys.exit(1)

    print(name)

    #template_dir = bitloop_root / "framework" / "templates" / "default"
    target_dir   = Path.cwd() / name

    # 1) Copy entire template_dir → target_dir
    # 2) Recursively replace "{{PROJECT_NAME}}" with `name` in text files
    # 3) Optionally `git init` and initial commit in target_dir

    print(f"Created new project '{name}' at {target_dir}")
