using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEditor;
using System.IO;

public class TestLevelLoad : MonoBehaviour {

	// Use this for initialization
    public Button m_loadLevel;
	void Start () 
    {
        Button btn = m_loadLevel.GetComponent<Button>();
        btn.onClick.AddListener(LoadLevel);
	}

    public void LoadLevel()
    {
        TextAsset m_textAsset = Resources.Load("level_1") as TextAsset;     
        if (m_textAsset == null) 
        {            
            Debug.Log ("File not found\n");
            return;
        } 
        ParseTMX tmx = new ParseTMX ();
        Transform brick = (Transform)AssetDatabase.LoadAssetAtPath ("Assets/Prefab/brick.prefab",typeof(Transform));
        if (brick == null) 
        {
            Debug.LogError ("Asset brick not found");
            return;
        }
        tmx.ParseFile (m_textAsset.text);
        for (int j = 0; j < tmx.m_height; j++)
            for (int i = 0; i < tmx.m_width; i++) 
            {
                int brick_id=tmx.m_brickData[j * tmx.m_width + i];
                if (brick_id != 0) 
                {
                    float x = 1.0f - i * 0.2f;
                    float y = 1.0f - j * 0.1f;
                    Instantiate (brick, new Vector3 (x, y, -0.168f), Quaternion.identity);
                }
            }
    }
		
}
