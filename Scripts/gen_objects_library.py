import xml.etree.ElementTree as ET
import struct

# Path to input XML and output binary file
xml_path = "../game/GameObjectsLibrary.xml"
bin_path = "../bin/GameObjects.bin"

# Internal tables
string_table = {}
string_list = []
string_index = {}

def add_string(s):
    if s not in string_table:
        idx = len(string_list)
        string_table[s] = idx
        string_list.append(s)
    return string_table[s]

# 1. Parse XML
tree = ET.parse(xml_path)
root = tree.getroot()

binary_data = bytearray()

# 2. Write header: placeholder for object count
object_count_pos = len(binary_data)
binary_data += struct.pack('<I', 0)  # Placeholder

objects = []

# 3. Build data
for obj in root.findall("object"):
    obj_name = obj.attrib["name"]
    obj_entry = {
        "name_idx": add_string(obj_name),
        "components": []
    }

    for comp in obj.findall("component"):
        comp_name = comp.attrib["name"]
        comp_entry = {
            "name_idx": add_string(comp_name),
            "properties": []
        }

        for prop in comp.findall("p"):
            prop_name = prop.attrib["name"]
            value_str = prop.attrib["value"]
            value_type = "string"

            # Try to auto-detect value type
            try:
                val = int(value_str)
                value_type = "int"
            except ValueError:
                val = value_str

            prop_entry = {
                "name_idx": add_string(prop_name),
                "type": value_type,
                "value": val if value_type == "int" else add_string(value_str)
            }

            comp_entry["properties"].append(prop_entry)
        obj_entry["components"].append(comp_entry)
    objects.append(obj_entry)

# 4. Now serialize objects
binary_data[object_count_pos:object_count_pos+4] = struct.pack('<I', len(objects))

for obj in objects:
    binary_data += struct.pack('<I', obj["name_idx"])
    binary_data += struct.pack('<I', len(obj["components"]))
    for comp in obj["components"]:
        binary_data += struct.pack('<I', comp["name_idx"])
        binary_data += struct.pack('<I', len(comp["properties"]))
        for prop in comp["properties"]:
            binary_data += struct.pack('<I', prop["name_idx"])
            binary_data += struct.pack('<B', 0 if prop["type"] == "int" else 1)
            if prop["type"] == "int":
                binary_data += struct.pack('<i', prop["value"])
            else:
                binary_data += struct.pack('<I', prop["value"])

# 5. Write string table
binary_data += struct.pack('<I', len(string_list))
for s in string_list:
    encoded = s.encode('utf-8')
    binary_data += struct.pack('<I', len(encoded))
    binary_data += encoded

# 6. Write to file
with open(bin_path, "wb") as f:
    f.write(binary_data)

print(f"Written {len(objects)} objects and {len(string_list)} strings to {bin_path}")
