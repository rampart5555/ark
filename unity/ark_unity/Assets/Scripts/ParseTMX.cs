using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using UnityEngine;

public class ParseTMX
{
    string m_filePath = @"c:\a_dev\ark_textures\level_1.tmx";
    int m_width;
    int m_height;
    List<int> m_brickData;
    List<int> m_powerupData;
    public ParseTMX()
    {
        m_brickData = new List<int>();
        m_powerupData = new List<int>();
    }
    public void ParseFile()
    {
        XmlDocument doc = new XmlDocument();
        doc.Load(m_filePath);
        XmlNode layer;
        XmlNode layer_data;
        /* bricks layer */
        layer = doc.DocumentElement.SelectSingleNode("//layer[@name='bricks']");
        if (layer == null)
        {
            Debug.Log("layer bricks not found");
            return;
        }            
        layer_data = layer.SelectSingleNode("./data");
        if(layer_data == null)
        {
            Debug.Log("bricks node data not found!");
            return;
        }
        foreach (string s in layer_data.InnerText.Split(','))            
            m_brickData.Add(int.Parse(s));

        /* powerup layer */
        layer = doc.DocumentElement.SelectSingleNode("//layer[@name='powerup']");
        if (layer == null)
        {
            Debug.Log("Node not found");
            return;
        }
        layer_data = layer.SelectSingleNode("./data");
        if (layer_data == null)
        {
            Debug.Log("bricks node data not found!");
            return;
        }
        foreach (string s in layer_data.InnerText.Split(','))
            m_powerupData.Add(int.Parse(s));

        m_width = int.Parse(layer.Attributes["width"].Value);
        m_height = int.Parse(layer.Attributes["height"].Value);
        Dump();
    }
    void Dump()
    {
        Debug.Log("Width:"  + m_width);
        Debug.Log("Height:" + m_height);
        Debug.Log("Layer bricks");
        for (int i = 0; i < m_height; i++)
        {
            for (int j = 0; j < m_width; j++)
            {
                Debug.Log(" "+m_brickData[i * m_width + j]);                    
            }
            Debug.Log("\n");
        }
        Debug.Log("Layer powerup");
        for (int i = 0; i < m_height; i++)
        {
            for (int j = 0; j < m_width; j++)
            {
                Debug.Log(" " + m_powerupData[i * m_width + j]);
            }
            Debug.Log("\n");
        }
    }
}

