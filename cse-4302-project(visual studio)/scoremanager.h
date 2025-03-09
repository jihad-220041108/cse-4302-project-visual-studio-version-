#pragma once
#include <bits/stdc++.h>
using namespace std;

class ScoreManager {
public:
    static void LoadScoresFromFile();
    static void SaveScoresToFile();
    static void AddScore(float score, string name);
    static vector<pair<float, string>> GetScores();

private:
    static vector<pair<float, string>> scores;
    static const string scoreFileName;
};
