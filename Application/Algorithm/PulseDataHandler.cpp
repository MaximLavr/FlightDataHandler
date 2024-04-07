#include "PulseDataHandler.h"

double PulseDataHandler::CalculateCurrentSpeedNSK()
{
    return sqrt(m_FlightData->G_Vx * m_FlightData->G_Vx + m_FlightData->G_Vy * m_FlightData->G_Vy +
        m_FlightData->G_Vz * m_FlightData->G_Vz);
}

double PulseDataHandler::CalculateCurrentSpeedSSK()
{
    return sqrt(m_FlightData->B_Vx * m_FlightData->B_Vx + m_FlightData->B_Vy * m_FlightData->B_Vy +
                m_FlightData->B_Vz * m_FlightData->B_Vz);
}

void PulseDataHandler::CalculateImpulses()
{
    for (int i = 0; i < COUNT_IMPULSE_TRAINS; i++)
    {
        for (int j = 0; j < COUNT_IMPULSES_IN_TRAIN; j++)
        {
            double lineMax = sqrt(pow(m_CosSignal1[i][j] - 2048, 2.0) + pow(m_SinSignal1[i][j] - 2048, 2.0));
            //double coefficient_AR = pow(10, m_FlightData->AR / 20.0);
            //double coefficient_ARU = pow(10, m_FlightData->ARU / 20.0);
            double coefficient_AR = m_FlightData->AR;
            double coefficient_ARU = m_FlightData->ARU;
            double coefficient_H = 1.0 / m_FlightData->Hrv;
            if (coefficient_AR == 0)
                coefficient_AR = 1;
            if (coefficient_ARU == 0)
                coefficient_ARU = 1;
            double coefficient = coefficient_AR * coefficient_ARU * coefficient_H;
            m_Impulses[i][j] = lineMax * coefficient;
            impulsesVector.push_back(m_Impulses[i][j]);

            /*double coef_AR_dB = m_Z_AR * (m_FlightData->AR + 1);
            double coef_AR = pow(10, (coef_AR_dB / 20));
            double coef_ARU_dB = Z_ARU * (m_FlightData->ARU + 1);
            double coef_ARU = pow(10, (coef_ARU_dB / 20));
            double coef_H = 1 / m_FlightData->Hrv;*/

            //double coef = coef_AR * coef_ARU * coef_H;
        }
    }
}

void PulseDataHandler::PrintImpulsesToFile(const char* filePath)
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
    file << "<------------Result Impulses BEGIN------------>" << std::endl;
    for (int i = 0; i < COUNT_IMPULSE_TRAINS; i++)
    {
        file << "[" << i + 1 << " s]";
        for (int j = 0; j < COUNT_IMPULSES_IN_TRAIN; j++)
        {
            file << "[" << j + 1 << " c]" << m_Impulses[i][j] << " ";
        }
        file << std::endl;
    }
    file << "<------------Result Impulses END------------>" << std::endl;
    file.close();
}

PulseDataHandler::PulseDataHandler(FlightData* flightData, std::vector<double> Z_AR, std::vector<double> Z_ARU)
{
    m_FlightData = flightData;
    m_Z_AR = std::move(Z_AR);
    m_Z_ARU = std::move(Z_ARU);

    m_CurrentSpeedSSK = CalculateCurrentSpeedSSK();
    m_CurrentSpeedNSK = CalculateCurrentSpeedNSK();
    m_CurrentHeightMeasured = m_FlightData->Hrv;
    m_CurrentHeightGPS = m_FlightData->G_Alt;
    m_CurrentFlightTime = m_FlightData->Trv;
    m_CurrentLatitude = m_FlightData->G_Lat;
    m_CurrentLongitude = m_FlightData->G_Long;
    m_CurrentYaw = m_FlightData->B_Head;
    m_CurrentPitch = m_FlightData->B_Pitch;
    m_CurrentRoll = m_FlightData->B_Roll;

    double AR_CoefficientDb = m_Z_AR[0] * (flightData->AR + 1);
    double ARU_CoefficientDb = m_Z_ARU[0] * (flightData->ARU + 1);

    for (int i = 0; i < m_FlightData->sizeSig_S1; i++)
    {
        int a = i / 63;
        int b = i % 63;
        m_CosSignal1[b][a] = m_FlightData->Sig_C1[i];
        m_SinSignal1[b][a] = m_FlightData->Sig_S1[i];
    }

    /*std::cout << "m_CosSignal1" << std::endl;
    for (int i = 0; i < COUNT_IMPULSE_TRAINS; i++)
    {
        for (int j = 0; j < COUNT_IMPULSES_IN_TRAIN; j++)
        {
            std::cout << m_CosSignal1[i][j] << ", ";
        }
        std::cout << std::endl;
    }
    std::cout << "m_SinSignal1" << std::endl;
    for (int i = 0; i < COUNT_IMPULSE_TRAINS; i++)
    {
        for (int j = 0; j < COUNT_IMPULSES_IN_TRAIN; j++)
        {
            std::cout << m_SinSignal1[i][j] << ", ";
        }
        std::cout << std::endl;
    }*/

    //PrintCurrentFlightData();
}
