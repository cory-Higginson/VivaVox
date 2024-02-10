import re
import sys

filename = "CMakeLists.txt"
version_pattern = re.compile(r'^set\s*\(\s*PROJECT_VERSION\s+(\d+)\.(\d+)\.(\d+)')

try:
    with open(filename, 'r+') as file:
        content = file.read()
        match = version_pattern.search(content)
        if match:
            major, minor, patch = match.groups()
            new_version = f"{major}.{minor}.{int(patch)+1}"
            print(f"::set-output name=new_version::{new_version}")  # Set output for GitHub Actions
        else:
            raise ValueError("PROJECT_VERSION not found in CMakeLists.txt")
except Exception as e:
    print(f"Error: {str(e)}")
    sys.exit(1)
