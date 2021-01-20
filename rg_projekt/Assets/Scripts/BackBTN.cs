using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class BackBTN : MonoBehaviour
{
    public void GoBack()
    {
        SceneManager.LoadScene("start");
    }

    public void TryAgain()
    {
        SceneManager.LoadScene("game");
    }
}
