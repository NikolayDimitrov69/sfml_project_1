import xml.etree.ElementTree as ET

def sanitize_enum_value(name):
    # Convert names to C++-safe identifiers
    return name.replace(" ", "_").replace("-", "_")

def generate_enum(name, items):
    enum_str = f"enum class E{name} : int {{\n"
    enum_str += "    Invalid = -1,\n"
    for i, item in enumerate(items):
        enum_name = sanitize_enum_value(item)
        enum_str += f"    {enum_name},\n"
    enum_str += f"    COUNT\n}};\n"
    return enum_str

def main():
    tree = ET.parse('../../game/ListLibrary.xml')
    root = tree.getroot()

    output = "// Auto-generated enum header file\n#pragma once\n\n"

    for list_elem in root.findall('list'):
        list_name = list_elem.get('name')
        items = [item.get('name') for item in list_elem.findall('listentry')]
        output += generate_enum(list_name, items) + "\n"

    with open("../../sfml-project2/GeneratedEnums.h", "w") as f:
        f.write(output)

    print("Generated GeneratedEnums.h")

if __name__ == "__main__":
    main()