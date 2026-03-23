#include "ScoreManager.h"
#include "Constants.h"

ScoreManager::ScoreManager() : m_score(0) {}

void ScoreManager::reset()         { m_score = 0; }
void ScoreManager::addBrickScore() { m_score += SCORE_PER_BRICK; }
int  ScoreManager::getScore() const { return m_score; }

std::string ScoreManager::getFormattedScore() const
{
    return "Score: " + std::to_string(m_score);
}
