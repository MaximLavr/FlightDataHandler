#pragma once
#include <vector>

class FlightData {
public:
    FlightData();
    double AR, ARU, B_Head, B_Pitch, B_Roll, B_Vx, B_Vy, B_Vz, G_Alt, G_Lat, G_Long, G_Vx, G_Vy, G_Vz, H0, Hrv, R0;

    /*double Sig_C1[63 * 341];
    double Sig_C2[63 * 341];
    double Sig_C3[63 * 341];

    double Sig_S1[63 * 341];
    double Sig_S2[63 * 341];
    double Sig_S3[63 * 341];

    double Swr[80];*/

    std::vector<double> Sig_C1;//[63 * 341];
    std::vector<double> Sig_C2;//[63 * 341];
    std::vector<double> Sig_C3;//[63 * 341];

    std::vector<double> Sig_S1;//[63 * 341];
    std::vector<double> Sig_S2;//[63 * 341];
    std::vector<double> Sig_S3;//[63 * 341];

    std::vector<double> Swr;//[80];

    double Trv;
    double dH;
    double tau;
    double tau_H;

    ~FlightData()
    {
    }
};
