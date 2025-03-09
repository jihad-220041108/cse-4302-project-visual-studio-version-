#pragma once
#include <bits/stdc++.h>
using namespace std;

class ScoreManager {
public:
    static void LoadScoresFromFile();
    static void SaveScoresToFile();
    static void AddScore(float score, string name);
    static vector<pair<string, float>> GetScores();

private:
    static vector<pair<string, float>> scores;
    static const string scoreFileName;
};
