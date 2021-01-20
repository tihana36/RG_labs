using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ballMovement : MonoBehaviour
{
    public bool start = false;
    public Rigidbody ball;
    public float ballSpeed = 800;

   

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKey("d"))
        {
            if (!start)
            {
                start = true;

                ball.AddForce(ballSpeed, ballSpeed, 0);
                unparent();
            }

        }

        else if (Input.GetKey("a"))
        {
            if (!start)
            {
                start = true;
                ball.AddForce(-ballSpeed, ballSpeed, 0);


                unparent();
            }
        }
 


        //check if ball bellow paddle

        if (ball.position.y < -13.5)
        {
            FindObjectOfType<GameManager>().lifeLost();
        }
    }

    void unparent() {
        ball.transform.SetParent(null);


    }

}
