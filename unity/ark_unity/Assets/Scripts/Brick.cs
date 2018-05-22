using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Brick : MonoBehaviour 
{
	public enum myEnum // your custom enumeration
	{
		brick_gold, 
		brick_silver, 
		brick_cooper
	};
	public myEnum m_brickType;

	void Start()
	{
		string txt = string.Format ("entity type {0}", m_brickType);
		Debug.Log (txt);
	}
}


