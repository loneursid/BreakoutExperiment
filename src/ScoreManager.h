#pragma once
#include <string>

class ScoreManager {
public:
    ScoreManager();

    void reset();
    void addBrickScore();
    int  getScore() const;

    // Returns "Score: N" for display
    std::string getFormattedScore() const;

private:
    int m_score;
};
