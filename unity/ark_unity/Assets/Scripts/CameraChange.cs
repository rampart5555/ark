using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraChange : MonoBehaviour {

	//the Bounds object we want the camera to follow
	public Bounds targetBounds;
	void Start()
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
	}
}
