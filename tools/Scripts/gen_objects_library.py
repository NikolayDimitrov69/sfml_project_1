import xml.etree.ElementTree as ET
import struct

# File paths
xml_path = "../../game/GameObjectsLibrary.xml"
components_path = "../../game/ComponentsLibrary.xml"
bin_path = "../../bin/GameObjects.bin"

# String table
string_table = {}
string_list = []

def add_string(s : str) -> int:
    string_table[s] = len(string_list)
    string_list.append(s)
    return string_table[s]

# --- Step 2: Serialize a property ---
def serialize_property(prop_name : str, prop_elem : ET.Element, defn : ET.Element) -> bytearray:
    output = bytearray()
    name_idx = add_string(prop_name)
    type = defn.get("type")
    output += struct.pack('<I', name_idx)

    if type == "int":
        output += struct.pack('<B', 0)
        output += struct.pack('<i', int(prop_elem.get("value", 0)))

    elif type == "bool":
        output += struct.pack('<B', 1)
        val = prop_elem.get("value", "false").lower() == "true"
        output += struct.pack('<B', 1 if val else 0)

    elif type == "float":
        output += struct.pack('<B', 2)
        output += struct.pack('<f', float(prop_elem.get("value", 0.0)))

    elif type == "string":
        output += struct.pack('<B', 3)
        val = prop_elem.get("value", "")
        output += struct.pack('<I', add_string(val))

    elif type == "struct":
        output += struct.pack('<B', 4)
        output += serialize_properties(prop_elem, defn)

    elif type == "array":
        output += struct.pack('<B', 5)
        output += serialize_properties(prop_elem, defn)

    return output

def serialize_properties(prop_elem : ET.Element, defn : ET.Element) -> bytearray:
    output = bytearray()
    properties = []
    # Gather serialized props
    for prop in prop_elem.findall("p"):
        prop_name = prop.get("name")
        property_template = find_element(defn.findall("p"), prop_name)
        if property_template == None:
            continue
        properties.append(serialize_property(prop_name, prop, property_template))

    # Write property count and data
    output += struct.pack('<I', len(properties))
    for prop_bytes in properties:
        output += prop_bytes

    return output

# --- Step 3: Parse and serialize game objects ---
game_tree = ET.parse(xml_path)
game_root = game_tree.getroot()

components_tree = ET.parse(components_path)
components_root = components_tree.getroot()

binary_data = bytearray()
object_count_pos = len(binary_data)
binary_data += struct.pack('<I', 0)  # Placeholder

objects = game_root.findall("object")
component_templates = components_root.findall("component")
binary_data[object_count_pos:object_count_pos+4] = struct.pack('<I', len(objects))

def find_element(container : list, key : str) -> ET.Element | None:
    return next((elem for elem in container if elem.get("name") == key), None)

for obj in objects:
    obj_name = obj.get("name")
    binary_data += struct.pack('<I', add_string(obj_name))

    components = obj.findall("component")
    binary_data += struct.pack('<I', len(components))

    for comp in components:
        comp_name = comp.get("name")
        binary_data += struct.pack('<I', add_string(comp_name))

        component_template = find_element(component_templates, comp_name)
        if component_template == None:
            continue

        binary_data += serialize_properties(comp, component_template)

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

