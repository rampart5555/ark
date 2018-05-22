using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Paddle : MonoBehaviour {

	// Use this for initialization
    private TargetJoint2D m_targetJoint;
	void Start () 
    {         
        m_targetJoint = gameObject.GetComponent<TargetJoint2D> ();
	}
	
	// Update is called once per frame
	void Update () 
    {        
        if (Input.GetMouseButton(0)) 
        {                        
            //Debug.Log ("Mouse click");
            Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
            RaycastHit hit;
            if (Physics.Raycast (ray, out hit)) 
            {                   
                m_targetJoint.target = new Vector2 (hit.point.x,hit.point.y);
            }                
        }		
	}
}

