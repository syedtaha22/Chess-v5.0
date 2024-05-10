#ifndef CALCULATE_ELO_H
#define CALCULATE_ELO_H

#include <cmath>

class CalculateELO {
private:
    double calculateExpectedScore(int PlayerARating, int PlayerBRating) const {

        /*      Expected Score = Qa / (Qa + Qb);

                    Qa = 10 ^ (engineRating / c);
                    Qb = 10 ^ (opponentRating / c);
                     c = Some Factor
        */

        const double c = 400.0;
        double Qa = pow(10, PlayerARating / c);
        double Qb = pow(10, PlayerBRating / c);
        double ExpectedScore = Qa / (Qa + Qb);

        return ExpectedScore;
    }

public:
    int operator()(int PlayerARating, int PlayerBRating, bool PlayerAWon) const {

        /*
                NewELO = OldELO + K * (Outcome — ExpectedOutcome),
                    k = Scalling Factor
        */

        double expectedOutcome = calculateExpectedScore(PlayerARating, PlayerBRating);
        double actualOutcome = PlayerAWon ? 1.0 : 0.0;
        const double kFactor = 42.0;

        return static_cast<int>(PlayerARating + kFactor * (actualOutcome - expectedOutcome));
    }
};

#endif // !CALCULATE_ELO_H


