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

# Build structure map of a component
def build_property_structure_map(component_elem):
    structure = {}
    for prop in component_elem.findall('p'):
        name = prop.get('name')
        children_map = build_property_structure_map(prop)
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
def sync_properties(game_object_component, template_component):
    game_object_component_props = {p.get('name'): p for p in game_object_component.findall('p')}

    for pname, template_component_info in template_component.items():
        is_array_type = template_component_info['element'].get('type') == 'array'

        if pname in game_object_component_props:
            if is_array_type:
                array_property = game_object_component_props[pname]
                for property in array_property.findall('p'):
                    template_component_children = template_component_info['children']
                    if property.get('name') in template_component_children:
                        sync_properties(property, template_component_children[property.get('name')]['children'])
                    else:
                        array_property.remove(property)
            else:
                sync_properties(game_object_component_props[pname], template_component_info['children'])
        else:
            game_object_component.append(copy.deepcopy(template_component_info['element'])) # adding of missing properties

    for pname in list(game_object_component_props.keys()):
        if pname not in template_component:
            game_object_component.remove(game_object_component_props[pname]) # removing of non-existent properties

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
