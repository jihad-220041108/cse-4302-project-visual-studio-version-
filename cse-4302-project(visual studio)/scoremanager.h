#pragma once
#include <bits/stdc++.h>
using namespace std;

class ScoreManager {
public:
    static void LoadScoresFromFile();
    static void SaveScoresToFile();
    static void AddScore(float score);
    static vector<float> GetTopScores();

private:
    static vector<float> scores;
    static const string scoreFileName;
};
