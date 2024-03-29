import re
import sys
import os

filename = "CMakeLists.txt"

# Patterns to match major, minor, and patch version lines
major_pattern = re.compile(r'set\(PROJECT_VERSION_MAJOR (\d+)\)')
minor_pattern = re.compile(r'set\(PROJECT_VERSION_MINOR (\d+)\)')
patch_pattern = re.compile(r'set\(PROJECT_VERSION_PATCH (\d+)\)')

try:
    with open(filename, 'r') as file:
        content = file.readlines()

    major, minor, patch = 0, 0, 0
    for line in content:
        if major_match := major_pattern.search(line):
            major = int(major_match.group(1))
        elif minor_match := minor_pattern.search(line):
            minor = int(minor_match.group(1))
        elif patch_match := patch_pattern.search(line):
            patch = int(patch_match.group(1))

    # Increment the patch version
    new_patch = patch + 1
    new_version = f"{major}.{minor}.{new_patch}"
    
    # Update the content with the new patch version
    updated_content = []
    for line in content:
        if patch_pattern.search(line):
            updated_content.append(f"set(PROJECT_VERSION_PATCH {new_patch})\n")
        else:
            updated_content.append(line)

    # Write the updated content back to CMakeLists.txt
    with open(filename, 'w') as file:
        file.writelines(updated_content)

    # Set the new version as an output variable for GitHub Actions
    github_output = os.getenv('GITHUB_ENV')
    if github_output:
        with open(github_output, 'a') as f:
            f.write(f"NEW_VERSION={new_version}\n")

except Exception as e:
    print(f"Error: {str(e)}")
    sys.exit(1)
