#include <iostream>
#include "../DataLoader/FlightData.h"
#include <cmath>
#include <utility>

class PulseDataHandler {
private:
    FlightData* m_FlightData;
    std::vector<double> m_Z_AR;
    std::vector<double> m_Z_ARU;

    double CalculateCurrentSpeedSSK();
    double CalculateCurrentSpeedNSK();

    double m_CurrentSpeedSSK;
    double m_CurrentSpeedNSK;
    double m_CurrentHeightMeasured;
    double m_CurrentHeightGPS;
    double m_CurrentFlightTime;

    double m_CurrentLatitude;
    double m_CurrentLongitude;

    double m_CurrentYaw;
    double m_CurrentPitch;
    double m_CurrentRoll;


    double m_AR_CoefficientDb;
    double m_ARU_CoefficientDb;
    double m_H_Coefficient;

    static const int COUNT_IMPULSES_IN_TRAIN = 341;
    static const int COUNT_IMPULSE_TRAINS = 63;

    double m_CosSignal1[COUNT_IMPULSE_TRAINS][COUNT_IMPULSES_IN_TRAIN];
    double m_SinSignal1[COUNT_IMPULSE_TRAINS][COUNT_IMPULSES_IN_TRAIN];
public:
    PulseDataHandler(FlightData* flightData, std::vector<double> Z_AR, std::vector<double> Z_ARU);
    void CalculateImpulses(); //COUNT_IMPULSE_TRAINS * COUNT_IMPULSES_IN_TRAIN
    void PrintImpulsesToFile(const char* filePath);
    std::vector<double> impulsesVector;

    double GetCurrentHeightMeasured() { return m_CurrentHeightMeasured; }
    double GetCurrentHeightGPS() { return m_CurrentHeightGPS; }
    double GetCurrentSpeedSSK() { return m_CurrentSpeedSSK; }
    double GetCurrentSpeedNSK() { return m_CurrentSpeedNSK; }
    double GetCurrentFlightTime() { return m_CurrentFlightTime; }
    double GetCurrentLatitude() { return m_CurrentLatitude; }
    double GetCurrentLongitude() { return m_CurrentLongitude; }
    double GetCurrentHead() { return m_CurrentYaw; }
    double GetCurrentPitch() { return m_CurrentPitch; }
    double GetCurrentRoll() { return m_CurrentRoll; }

    double m_Impulses[COUNT_IMPULSE_TRAINS][COUNT_IMPULSES_IN_TRAIN];
};

