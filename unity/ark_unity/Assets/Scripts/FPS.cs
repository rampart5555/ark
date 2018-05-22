using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class FPS : MonoBehaviour 
{	
	float deltaTime = 0.0f;
    private Text m_text;
	void Start () 
    {
        m_text = GetComponent<Text> ();
	}
	
	// Update is called once per frame
	void Update () 
	{
		deltaTime += (Time.unscaledDeltaTime - deltaTime) * 0.1f;		
	}
	void OnGUI()
	{
		float msec = deltaTime * 1000.0f;
		float fps = 1.0f / deltaTime;
		string text = string.Format("{0:0.0} ms ({1:0.} fps)", msec, fps);	
        m_text.text = text;
	}
}
