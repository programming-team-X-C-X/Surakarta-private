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
        : game(game), player(game.GetGameInfo()->current_player_),/* agent_(std::make_shared<SurakartaAgentMine>(game.GetBoard(), game.GetGameInfo(), game.GetRuleManager())),*/  fromPos(fromPos),HintVector(to_pos), depth(depth),shouldExit(false) {}
    void requestStop() {
        shouldExit = true;
    }

public slots:
    void doWork() {
        // while(!shouldExit){
        for (auto toPos : HintVector) {
            if(shouldExit) break;
            // qDebug() << "calculating to " << toPos.x << toPos.y << Qt::endl;
            SurakartaGame *game_ = new SurakartaGame(game);
            // qDebug() << "current player is " << (game_->GetGameInfo()->current_player_==SurakartaPlayer::BLACK ? "black" : "white") << Qt::endl;
            SurakartaMove move_ = SurakartaMove(fromPos, toPos, player);
            if (game_->Move(move_).IsLegal())
                qDebug() << "the initial move is " << move_.from.x << move_.from.y << move_.to.x << move_.to.y << Qt::endl;

            for (int i = 0; i < depth*2; i++) {
                // qDebug() << "searching" << Qt::endl;
                if(checkEnd(*game_)) {
                    dangerousPos.push_back(toPos);
                    break;
                }
                if (i + 1 != depth * 2) {
                    // qDebug() << "current player is " << (game_->GetGameInfo()->current_player_==SurakartaPlayer::BLACK ? "black" : "white") << Qt::endl;
                    move_ = game_->agent_->CalculateMove();
                    // qDebug() << "calculated move is " << move_.from.x << move_.from.y << move_.to.x << move_.to.y << Qt::endl;
                }
            }
        }

        // qDebug() << "finish calculate" << Qt::endl;
        emit resultReady(dangerousPos);
    }
public:
    bool checkEnd(SurakartaGame game) {
        // qDebug() << "calculating" << Qt::endl;
        for(unsigned i = 0; i < game.GetBoardSize(); i++ ){
            for(unsigned j = 0; j < game.GetBoardSize(); j++){
                if((*game.GetRuleManager()->board_)[i][j]->GetColor() == player)
                {
                    // qDebug() << "find player in" << i << j << Qt::endl;
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
    int depth;
    std::atomic<bool> shouldExit{false};
};

#endif // AI_TASK_H
