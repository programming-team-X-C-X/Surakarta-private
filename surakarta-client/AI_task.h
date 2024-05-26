#ifndef AI_TASK_H
#define AI_TASK_H

#include "agent_mine.h"
#include "info_game.h"

class AITask : public QObject {
    Q_OBJECT

public:
    AITask(std::shared_ptr<SurakartaAgentMine> agent)
        : agent_(agent) {}

public slots:
    void doWork() {
        if (agent_)  {
            SurakartaMove move = agent_->CalculateMove();
            emit resultReady(move);
        }
    }

signals:
    void resultReady(const SurakartaMove &move);

private:
    std::shared_ptr<SurakartaAgentMine> agent_;
};

class DEADCalculator : public QObject {
    Q_OBJECT

public:
    DEADCalculator(SurakartaGame game, SurakartaPosition fromPos, std::vector<SurakartaPosition> to_pos, int depth)
        : game(game), player(game.GetGameInfo()->current_player_),/* agent_(std::make_shared<SurakartaAgentMine>(game.GetBoard(), game.GetGameInfo(), game.GetRuleManager())),*/  fromPos(fromPos),HintVector(to_pos), depth(depth) {}

public slots:
    void doWork() {
        for (auto toPos : HintVector) {
            SurakartaGame game_ = game;
            SurakartaMove move_ = SurakartaMove(fromPos, toPos, player);
            SurakartaAgentMine agent_ = SurakartaAgentMine(game_.GetBoard(), game_.GetGameInfo(), game_.GetRuleManager());

            for (int i = 0; i < depth; i++) {
                game_.Move(move_);
                if(calculator(game_)) {
                    dangerousPos.push_back(toPos);
                    break;
                }
                move_ = agent_.CalculateMove();
            }
            // emit resultReady(toPos, true);
        }
        emit resultReady(dangerousPos);
        qDebug() << "finish calculate" << Qt::endl;
    }
public:
    bool calculator(SurakartaGame game) {
        qDebug() << "calculating" << Qt::endl;
        for(unsigned i = 0; i < game.GetBoardSize(); i++ ){
            for(unsigned j = 0; j < game.GetBoardSize(); j++){
                if((*game.GetRuleManager()->board_)[i][j]->GetColor() == player)
                {
                    return false;
                }
            }
        }
        return true; // lose
    }


signals:
    void resultReady(std::vector<SurakartaPosition>);

private:
    SurakartaGame game;
    SurakartaPlayer player;
    SurakartaPosition fromPos;
    std::vector<SurakartaPosition> HintVector;
    std::vector<SurakartaPosition> dangerousPos;
    // SurakartaRuleManager rule_manager;
    int depth;
};



#endif // AI_TASK_H
