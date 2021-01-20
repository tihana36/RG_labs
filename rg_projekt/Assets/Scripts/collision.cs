using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class collision : MonoBehaviour
{

    public ParticleSystem effect;
    void OnCollisionEnter(Collision collision)
    {        

        Vector3 position = transform.position;
        Destroy(gameObject);
        FindObjectOfType<GameManager>().brickDestroy();

        Instantiate(effect, position, Quaternion.identity);

    }
}
