using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class player : MonoBehaviour
{
    //public Rigidbody rb;
    //public float movementForce = 500;

    private Vector3 playerPosition = new Vector3(0, -12.5f, 0);
    void FixedUpdate() {
        if (Input.GetKey(KeyCode.RightArrow))
        {
            float x = transform.position.x + 1f;

            if (transform.position.x < 25) {
                playerPosition.x = x;
                transform.position = playerPosition;
            }

            playerPosition.x = Mathf.Clamp(x, -25f, 25f);
           
            //rb.AddForce(movementForce * Time.deltaTime, 0, 0);
        }
        else if (Input.GetKey(KeyCode.LeftArrow)) {
            float x = transform.position.x - 1f;

            if (transform.position.x > -25)
            {
                playerPosition.x = x;
                transform.position = playerPosition;
            }
            //rb.AddForce(-movementForce * Time.deltaTime, 0, 0);
        }

        
        
    }
}
