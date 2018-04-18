using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Cube : MonoBehaviour 
{
	public float floatHeight;
	public float liftForce;
	public float damping;
	public Rigidbody2D rb2D;
	public float distance = 50f;
	void Start() 
	{
		rb2D = GetComponent<Rigidbody2D>();
	}

	void Update()
	{
		if (Input.GetMouseButtonDown (0)) 
		{
			Debug.Log ("Pressed primary button.");
		}
		
	}

	void FixedUpdate() 
	{
		//if mouse button (left hand side) pressed instantiate a raycast
		if(Input.GetMouseButtonDown(0))
		{
			//create a ray cast and set it to the mouses cursor position in game
			Ray ray = Camera.main.ScreenPointToRay (Input.mousePosition);
			RaycastHit hit;
			if (Physics.Raycast (ray, out hit, distance)) 
			{
				//draw invisible ray cast/vector
				Debug.DrawLine (ray.origin, hit.point);
				Debug.DrawLine(ray.origin, hit.point, Color.white,1000,true); 
				//log hit area to the console
				Debug.Log(hit.point);
				//rb2D.AddForce(new Vector2(3.0f,3.0f));
				rb2D.AddForce(transform.up * 5.0f);

			}    
		}    
		/*		
		RaycastHit2D hit = Physics2D.Raycast(transform.position, -Vector2.up);
		if (hit.collider != null) 
		{
			Debug.Log ("kaka");
			float distance = Mathf.Abs(hit.point.y - transform.position.y);
			float heightError = floatHeight - distance;
			float force = liftForce * heightError - rb2D.velocity.y * damping;
			rb2D.AddForce(Vector3.up * force);
		}
		*/

	}

}
