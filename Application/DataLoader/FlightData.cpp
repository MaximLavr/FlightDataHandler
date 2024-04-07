#include "FlightData.h"

FlightData::FlightData() {}

void FlightData::PrintAllToFile(const char* filePath)
{
    std::ofstream file(filePath, std::ios::app);

    file << std::endl;
    file << std::endl;
    file << std::endl;
    file << std::endl;
    file << std::endl;
    file << std::endl;
    file << std::endl;
    file << std::endl;
    file << std::endl;
    file << std::endl;

    file << "<------------All flight data BEGIN------------>" << std::endl;

    file << "Point number: " << pointNumber << std::endl;

    file << "AR: " << AR << std::endl;
    file << "ARU: " << ARU << std::endl;

    file << "B_Head: " << B_Head << std::endl;
    file << "B_Pitch: " << B_Pitch << std::endl;
    file << "B_Roll: " << B_Roll << std::endl;

    file << "B_Vx: " << B_Vx << std::endl;
    file << "B_Vy: " << B_Vy << std::endl;
    file << "B_Vz: " << B_Vz << std::endl;

    file << "G_Alt: " << G_Alt << std::endl;
    file << "G_Lat: " << G_Lat << std::endl;
    file << "G_Long: " << G_Long << std::endl;

    file << "G_Vx: " << G_Vx << std::endl;
    file << "G_Vy: " << G_Vy << std::endl;
    file << "G_Vz: " << G_Vz << std::endl;

    file << "H0: " << H0 << std::endl;
    file << "Hrv: " << Hrv << std::endl;
    file << "R0: " << R0 << std::endl;

    file << "Sig_C1 (size = " << Sig_C1.size() << "): ";
    for (auto& i : Sig_C1)
    {
        file << i << " ";
    }
    file << std::endl;

    file << "Sig_C2 (size = " << Sig_C2.size() << "): ";
    for (auto& i : Sig_C2)
    {
        file << i << " ";
    }
    file << std::endl;

    file << "Sig_C3 (size = " << Sig_C3.size() << "): ";
    for (auto& i : Sig_C3)
    {
        file << i << " ";
    }
    file << std::endl;

    file << "Sig_S1 (size = " << Sig_S1.size() << "): ";
    for (auto& i : Sig_S1)
    {
        file << i << " ";
    }
    file << std::endl;

    file << "Sig_S2 (size = " << Sig_S2.size() << "): ";
    for (auto& i : Sig_S2)
    {
        file << i << " ";
    }
    file << std::endl;

    file << "Sig_S3 (size = " << Sig_S3.size() << "): ";
    for (auto& i : Sig_S3)
    {
        file << i << " ";
    }
    file << std::endl;

    file << "Swr (size = " << Swr.size() << "): ";
    for (auto& i : Swr)
    {
        file << i << " ";
    }
    file << std::endl;

    file << "Trv: " << Trv << std::endl;
    file << "dH: " << dH << std::endl;
    file << "tau: " << tau << std::endl;
    file << "tau_H: " << tau_H << std::endl;

    file << "<------------All flight data END------------>" << std::endl;

    file.close();
}
