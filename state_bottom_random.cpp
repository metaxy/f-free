#include "state_bottom_random.h"
#include "vf.h"
#include "randomize.h"
#include "forbidden.h"
#include <math.h>
StateBottomRandom::StateBottomRandom(Config conf) : State(conf), m_countIteration(0), m_validChanges(0), m_invalidChanges(0), m_skipBecauseOfWeight(0)
{
}
MGraph StateBottomRandom::solve()
{
    MGraph input(m_input);
    MGraph weighted;
    input.clear();
    while(true) {
        m_countIteration++;
        if(this->getInt("useWeight", 1) == 1) {
            weighted = Forbidden::forbiddenWeight(&m_input, m_forbidden);
        }

        vector<Edge> diff = r->randomVector(m_input.difference(&input));
        bool oneChange = false;
        for(const Edge &e: diff) {

            if(this->getInt("useWeight", 1) == 1 && !r->choice((atan(weighted.getWeight(e)*0.5)/(M_PI/2)))) {
                m_skipBecauseOfWeight++;
                continue;
            }

            input.flip(e);
            if(!isValid(&input)) {
                input.flip(e);
                m_invalidChanges++;
            } else {
                oneChange = true;
                m_validChanges++;
            }
        }
        if(!oneChange) {
            break;
        }

    }
    return input;
}

void StateBottomRandom::final()
{
    m_debug["countIteration"] = std::to_string(m_countIteration);
    m_debug["validChanges"] = std::to_string(m_validChanges);
    m_debug["invalidChanges"] = std::to_string(m_invalidChanges);
    m_debug["skipBecauseOfWeight"] = std::to_string(m_skipBecauseOfWeight);
}
