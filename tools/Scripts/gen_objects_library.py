import xml.etree.ElementTree as ET
import struct
import re

# File paths
xml_path = "../../game/GameObjectsLibrary.xml"
components_path = "../../game/ComponentsLibrary.xml"
bin_path = "../../bin/GameObjects.bin"

# String table
string_table = {}
string_list = []

def add_string(s):
    if s not in string_table:
        string_table[s] = len(string_list)
        string_list.append(s)
    return string_table[s]

# --- Step 1: Load component types ---
def extract_type_tree(elem):
    tree = {}
    for p in elem.findall("p"):
        name = p.get("name")
        typ = p.get("type", "string")
        subtree = extract_type_tree(p)
        tree[name] = {
            "type": typ,
            "children": subtree
        }
    return tree

components_tree = ET.parse(components_path)
components_root = components_tree.getroot()

component_types = {}
for comp in components_root.findall("component"):
    name = comp.get("name")
    component_types[name] = extract_type_tree(comp)

# --- Step 2: Serialize a property ---
def serialize_property(prop_name, prop_elem, defn):
    output = bytearray()
    name_idx = add_string(prop_name)
    typ = defn["type"]
    output += struct.pack('<I', name_idx)

    if typ == "int":
        output += struct.pack('<B', 0)
        output += struct.pack('<i', int(prop_elem.get("value", 0)))

    elif typ == "bool":
        output += struct.pack('<B', 1)
        val = prop_elem.get("value", "false").lower() == "true"
        output += struct.pack('<B', 1 if val else 0)

    elif typ == "float":
        output += struct.pack('<B', 2)
        output += struct.pack('<f', float(prop_elem.get("value", 0.0)))

    elif typ == "string":
        output += struct.pack('<B', 3)
        val = prop_elem.get("value", "")
        output += struct.pack('<I', add_string(val))

    elif typ == "struct":
        output += struct.pack('<B', 4)
        output += struct.pack('<I', len(defn["children"]))
        for child_name, child_def in defn["children"].items():
            child_elem = prop_elem.find(f"./p[@name='{child_name}']")
            if child_elem is None:
                child_elem = ET.Element("p", {"name": child_name, "value": ""})
            output += serialize_property(child_name, child_elem, child_def)

    return output

# --- Step 3: Parse and serialize game objects ---
game_tree = ET.parse(xml_path)
game_root = game_tree.getroot()

binary_data = bytearray()
object_count_pos = len(binary_data)
binary_data += struct.pack('<I', 0)  # Placeholder

objects = game_root.findall("object")
binary_data[object_count_pos:object_count_pos+4] = struct.pack('<I', len(objects))

for obj in objects:
    obj_name = obj.get("name")
    binary_data += struct.pack('<I', add_string(obj_name))

    components = obj.findall("component")
    binary_data += struct.pack('<I', len(components))

    for comp in components:
        comp_name = comp.get("name")
        binary_data += struct.pack('<I', add_string(comp_name))

        type_map = component_types.get(comp_name, {})
        properties = []

        # Gather serialized props
        for prop in comp.findall("p"):
            prop_name = prop.get("name")

            # Handle array
            defn = type_map.get(prop_name)
            if defn and defn["type"] == "array":
                array_elem_def = list(defn["children"].values())[0]  # struct or type inside array
                array_elem_name = list(defn["children"].keys())[0]
                for entry in prop.findall("p"):
                    entry_name = entry.get("name", "")
                    if re.match(rf'^{array_elem_name}_[0-9]+$', entry_name):
                        properties.append(serialize_property(entry_name, entry, array_elem_def))
            else:
                if prop_name in type_map:
                    properties.append(serialize_property(prop_name, prop, type_map[prop_name]))

        # Write property count and data
        binary_data += struct.pack('<I', len(properties))
        for prop_bytes in properties:
            binary_data += prop_bytes

# --- Step 4: Write string table ---
binary_data += struct.pack('<I', len(string_list))
for s in string_list:
    encoded = s.encode('utf-8')
    binary_data += struct.pack('<I', len(encoded))
    binary_data += encoded

# --- Step 5: Write to file ---
with open(bin_path, "wb") as f:
    f.write(binary_data)

print(f"✅ Written {len(objects)} objects and {len(string_list)} strings to {bin_path}")

