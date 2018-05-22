using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Ball : MonoBehaviour {

    public Vector2 m_speed;
	public AudioClip m_brickHit;

	void Start () 
    {
        Rigidbody2D rb=gameObject.GetComponent<Rigidbody2D>();
        rb.velocity = m_speed;		
	}
	
	// Update is called once per frame
	void Update () 
    {
		
	}

	void OnCollisionEnter2D(Collision2D col)
	{
		//Debug.Log (col.gameObject.name);
		AudioSource.PlayClipAtPoint (m_brickHit, transform.position);
			
	}
}
