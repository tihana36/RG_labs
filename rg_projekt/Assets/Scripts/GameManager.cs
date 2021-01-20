using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;
using TMPro;
public class GameManager : MonoBehaviour
{
    
    bool gameOver = false;

    public GameObject ball;
    public GameObject ball2;
    public GameObject ball3;


    int numBricks = 60;

    public GameObject gameOverUI;
    public GameObject livesPanel;


    //public GameObject displayScore;

    
    // public GameObject scoreUI;

    public TextMeshProUGUI scoreUI;

    public TextMeshProUGUI gameOverScore;

    public TextMeshProUGUI numLivesUI;

    int lives = 3;


    int score = 0;

   // private GameObject cloneBall;


    

    public void lifeLost() {
        lives--;
        
        numLivesUI.text = "Lives: "+lives.ToString();

        if (lives == 2) {
            Destroy(ball);
            ball2.SetActive(true);
        }

        else if(lives == 1)
        {
            Destroy(ball2);
            ball3.SetActive(true);
        }

        if (lives < 1) {
            EndGame();
        }
          
    }

    public void brickDestroy() {
        numBricks--;
        score += 15;

        
        scoreUI.text = "Score: " + score.ToString();
        if (numBricks < 1) {
            Victory();
        }
    }

    public void gameOverText() {
        gameOverUI.SetActive(true);
        livesPanel.SetActive(false);
        gameOverScore.text = "Your score: "+score.ToString();
    }

    public void EndGame() {
        if (!gameOver) {
            gameOver = true;
            //Debug.Log("GAME OVER");
            gameOverText();
        }
    }


    

    public void Victory() {
        
            SceneManager.LoadScene("victory");
        
    }


}
