import xml.etree.ElementTree as ET
Ep_1 = [
    "Level_01.tmx",
    "Level_02.tmx",
    "Level_03.tmx",
    "Level_04.tmx",
    "Level_05.tmx",
    "Level_06.tmx",
]

def buildLevel(fname):
    tree = ET.parse(fname)
    root = tree.getroot()
    layer_list = root.findall("./layer")
    level_name = fname.split(".")[0]
    indent="    ";
    string_data =  level_name + "={"+"\n"
    string_data += "{}name = \"{}\",\n".format(indent,level_name)
    string_data += "{}width = {},\n".format(indent,11)
    string_data += "{}height = {},\n".format(indent,17)
    _index=0
    for layer in layer_list:
        data=layer.find("./data")
        data_lines=data.text.splitlines()
        
        string_data += indent+"%s={"%layer.attrib["name"] 
        for line in data_lines:
            string_data += indent*2 + line + "\n"
        string_data += indent+"}" 
        if(_index==0):
            string_data += ","
        string_data += "\n"
        _index += 1
    string_data += "}"  
    print string_data 
    
def buildEpisode(ep_name):
    for ep in Ep_1:
        buildLevel(ep)

buildEpisode("episode_1")    
    

