#pragma once
#include <vector>
#include <iostream>
#include <fstream>

class FlightData {
public:
    FlightData();
    int pointNumber;

    int sizeSig_C1, sizeSig_C2, sizeSig_C3, sizeSig_S1, sizeSig_S2, sizeSig_S3, sizeSwr;
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

    std::vector<double> Swr;//[???];

    double Trv;
    double dH;
    double tau;
    double tau_H;

    ~FlightData()
    {
    }

    void PrintAllToFile(const char* filePath);
};
