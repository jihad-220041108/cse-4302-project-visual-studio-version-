#include "scoremanager.h"

const string ScoreManager::scoreFileName = "scores.txt"; 
vector<pair<float, string>> ScoreManager::scores;

void ScoreManager::LoadScoresFromFile() {
    ifstream file(scoreFileName);
    string name;
    float score;
    while (file >> name >> score) {
        scores.push_back({score, name});
    }
    file.close();
}

void ScoreManager::SaveScoresToFile() {
    ofstream file(scoreFileName);
    file.clear();
    for (const auto& score : scores) {
        file << score.second << " " << score.first << endl;
    }
    file.close();
}

void ScoreManager::AddScore(float score, string name) {
    scores.push_back({score, name});
    sort(scores.rbegin(), scores.rend());
    if (scores.size() > 5) {
        scores.pop_back(); // Keep only top 5 scores
    }
}

vector<pair<float, string>> ScoreManager::GetScores() {
    LoadScoresFromFile();
    vector<pair<float, string>> temp = scores;
    scores.clear();
    return temp;
}
