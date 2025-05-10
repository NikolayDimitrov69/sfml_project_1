import xml.etree.ElementTree as ET
import copy
import os
import re
from xml.dom import minidom

# File paths
COMPONENTS_FILE = '../../game/ComponentsLibrary.xml'
GAMEOBJECTS_FILE = '../../game/GameObjectsLibrary.xml'

# Load XMLs
components_tree = ET.parse(COMPONENTS_FILE)
components_root = components_tree.getroot()
gameobjects_tree = ET.parse(GAMEOBJECTS_FILE)
gameobjects_root = gameobjects_tree.getroot()

# Clone a property element (preserve 'type' during sync)
def clone_property_structure(prop_elem):
    clone = ET.Element('p', {
        k: v for k, v in prop_elem.attrib.items() if k in ['name', 'value', 'type']
    })
    for child in prop_elem:
        clone.append(clone_property_structure(child))
    return clone

# Build structure map of a component, with support for array element types
def build_property_structure_map(component_elem):
    structure = {}
    for prop in component_elem.findall('p'):
        name = prop.get('name')
        children_map = build_property_structure_map(prop)

        if prop.get('type') == 'array':
            # Handle array: define the structure of its internal element
            array_struct_elem = prop.find('p')
            if array_struct_elem is not None:
                element_name = array_struct_elem.get('name')
                children_map = build_property_structure_map(array_struct_elem)
                structure[name] = {
                    'element': clone_property_structure(prop),
                    'children': children_map,
                    'array_element_name': element_name
                }
                continue

        structure[name] = {
            'element': clone_property_structure(prop),
            'children': children_map
        }
    return structure

component_structures = {
    comp.get('name'): build_property_structure_map(comp)
    for comp in components_root.findall('component')
}

# Sync properties with full support for arrays and _number entries
def sync_properties(parent_elem, definition_structure, is_array=False, array_element_name=None):
    current_props = {p.get('name'): p for p in parent_elem.findall('p')}

    for pname, def_info in definition_structure.items():
        is_array_type = def_info['element'].get('type') == 'array'
        array_elem_name = def_info.get('array_element_name')

        if pname in current_props:
            if is_array_type:
                array_elem = current_props[pname]
                for child in array_elem.findall('p'):
                    cname = child.get('name')
                    if cname and array_elem_name and re.match(rf'^{array_elem_name}_[0-9]+$', cname):
                        sync_properties(child, def_info['children'], is_array=True)
            else:
                sync_properties(current_props[pname], def_info['children'])
        else:
            parent_elem.append(copy.deepcopy(def_info['element']))

    for pname in list(current_props.keys()):
        if pname not in definition_structure:
            if is_array and array_element_name and re.match(rf'^{array_element_name}_[0-9]+$', pname):
                continue  # Preserve numbered array entries
            parent_elem.remove(current_props[pname])

# Strip 'type' attributes before saving
def strip_type_attributes(elem):
    if 'type' in elem.attrib:
        del elem.attrib['type']
    for child in elem:
        strip_type_attributes(child)

# Apply to game objects
for obj in gameobjects_root.findall('object'):
    for comp in obj.findall('component'):
        cname = comp.get('name')
        if cname in component_structures:
            sync_properties(comp, component_structures[cname])
        for prop in comp.iter('p'):
            strip_type_attributes(prop)

# Pretty print using minidom
raw_string = ET.tostring(gameobjects_root, encoding='utf-8')
pretty_xml = minidom.parseString(raw_string).toprettyxml(indent="\t")

# Clean up excess blank lines
pretty_xml = "\n".join([line for line in pretty_xml.split('\n') if line.strip()])

# Save to original file
with open(GAMEOBJECTS_FILE, 'w', encoding='utf-8') as f:
    f.write(pretty_xml)

print("✅ GameObjectsLibrary.xml synchronized and saved.")
