using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using UnityEngine;

public class ParseTMX
{    
    public int m_width;
    public int m_height;
    public List<int> m_brickData;
    public List<int> m_powerupData;

    public ParseTMX()
    {
        m_brickData = new List<int>();
        m_powerupData = new List<int>();
    }
    public void ParseLayerNode(XmlDocument xmlDoc,string layerName)
    {
        XmlNode layer;
        XmlNode layer_data;
        string xpath = string.Format("//layer[@name='{0}']",layerName);
        Debug.Log (xpath);
        layer = xmlDoc.DocumentElement.SelectSingleNode(xpath);

        if (layer == null)
        {
            Debug.LogError("Layer not found");
            return;
        }            
        layer_data = layer.SelectSingleNode("./data");
        if(layer_data == null)
        {
            Debug.LogError("Data not found!");
            return;
        }

        if (layerName == "bricks") 
        {
            foreach (string s in layer_data.InnerText.Split(','))
                m_brickData.Add (int.Parse (s));
        } 
        else if (layerName == "powerup") 
        {
            foreach (string s in layer_data.InnerText.Split(','))
                m_powerupData.Add (int.Parse (s));
        }
    }

    public void ParseFile(string xml_string)
    {
        XmlDocument doc = new XmlDocument();
        doc.LoadXml(xml_string);
        XmlNode mapNode = doc.DocumentElement.SelectSingleNode("//map");
        if (mapNode != null) 
        {
            m_width = int.Parse(mapNode.Attributes["width"].Value);
            m_height = int.Parse(mapNode.Attributes["height"].Value);
        }
        ParseLayerNode (doc, "bricks");
        ParseLayerNode (doc, "powerup");
        //Dump();
    }
    void Dump()
    {
        Debug.Log("Width:"  + m_width);
        Debug.Log("Height:" + m_height);
        Debug.Log("Layer bricks");
        string brickStr="";
        for (int i = 0; i < m_height; i++)
        {
            for (int j = 0; j < m_width; j++)
            {
                brickStr += " "+m_brickData[i * m_width + j];                    
            }
            brickStr += "\n";
        }
        Debug.Log(brickStr);

        Debug.Log("Layer powerup");
        string powerupStr="";
        for (int j = 0; j < m_height; j++)
        {
            for (int i = 0; i < m_width; i++)
            {
                powerupStr += " "+m_brickData[j * m_width + i];                    
            }
            powerupStr += "\n";
        }
        Debug.Log (powerupStr);
    }
}

