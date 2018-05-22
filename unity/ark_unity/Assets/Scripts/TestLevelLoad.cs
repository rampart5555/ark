using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

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
        Debug.Log ("Load level");       
        //string levelPath =  Application.dataPath.ToString() + "/Levels/" + level;
        //string levelPath = level;
        //Debug.Log(levelPath);
        Object m_textAsset = Resources.Load("level_1");
        //TextAsset m_textAsset= Resources.Load("level_1") as TextAsset;
        if (m_textAsset != null) 
        {
            Debug.Log ("File found\n");
        } 
        else 
        {
            Debug.Log ("File not found\n");
        }
        //ParseTMX tmx = new ParseTMX ();
        //string [] results = AssetDatabase.FindAssets("level_1");
        TextAsset [] ta_arr = Resources.FindObjectsOfTypeAll<TextAsset>();
        foreach(TextAsset ta in ta_arr)
        {
            //print (ta.name);
        }
    }
		
}
