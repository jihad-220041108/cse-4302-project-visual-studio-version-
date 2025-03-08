#include "scoremanager.h"

const string ScoreManager::scoreFileName = "scores.txt"; 
vector<float> ScoreManager::scores;

void ScoreManager::LoadScoresFromFile() {
    ifstream file(scoreFileName);
    float score;
    while (file >> score) {
        scores.push_back(score);
    }
    file.close();
}

void ScoreManager::SaveScoresToFile() {
    ofstream file(scoreFileName);
    for (const float& score : scores) {
        file << score << std::endl;
    }
    file.close();
}

void ScoreManager::AddScore(float score) {
    scores.push_back(score);
    sort(scores.rbegin(), scores.rend());
    if (scores.size() > 5) {
        scores.pop_back(); // Keep top 5 scores
    }
}

vector<float> ScoreManager::GetTopScores() {
    return scores;
}
