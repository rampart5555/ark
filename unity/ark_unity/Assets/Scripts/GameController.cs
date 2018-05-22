using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameController : MonoBehaviour 
{
	public Bounds targetBounds;
	void Start()
	{		
		CameraSizeChange ();
	}
	void CameraSizeChange()
	{
		float screenRatio = (float)Screen.width / (float)Screen.height;
		float targetRatio = targetBounds.size.x / targetBounds.size.y;

		if (screenRatio >= targetRatio)
		{
			Camera.main.orthographicSize = targetBounds.size.y / 2;
		}
		else
		{
			float differenceInSize = targetRatio / screenRatio;
			Camera.main.orthographicSize = targetBounds.size.y / 2 * differenceInSize;
		}
		transform.position = new Vector3(targetBounds.center.x, targetBounds.center.y, -1f);
		string str = string.Format ("CameraSize: {0}", Camera.main.orthographicSize);
	}
}
