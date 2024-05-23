#ifndef AI_TASK_H
#define AI_TASK_H

#include "agent_random.h"
#include "agent_mine.h"

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

#endif // AI_TASK_H
