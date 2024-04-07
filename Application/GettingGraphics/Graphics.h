#include <cmath>
#include <matplot/matplot.h>
#include <algorithm>
#include <vector>
#include <random>
#include <numeric>
#define _USE_MATH_DEFINES
#include <math.h>

//using namespace matplot;

namespace app_graphics
{
    using namespace std;
    using namespace matplot;

    class Graphics {
    private:
    public:
        std::vector<double> TimeValues;
        std::vector<double> AltValues;
        std::vector<double> ImpulsesValues;
        std::string AltGraphPath;
        std::string ImpulseGraphPath;

        std::vector<double> WaterEthalonImpulses;
        std::vector<double> ForestEthalonImpulses;
        std::vector<double> FieldEthalonImpulses;
        std::vector<double> PlowedFieldEthalonImpulses;
        std::vector<double> water;
        std::vector<double> forest;

        Graphics(/*std::vector<double>& timeValues, std::vector<double>& altValues,*/ std::vector<double>& impulsesValues);
        std::string GetSurfaceType();

        std::vector<double> x11, x21, y11, y21;
        //vector<vector<double>> impulse;
        std::string decision;

        void DrawAltGraph(std::vector<double>& alt, std::vector<double>& time);
        void DrawImpulseGraph(std::vector<double>& impulses);
        void DrawImpulseHist(std::vector<double>& impulses);
        std::vector<double> ReadImpulsesFromFile(const char* filePath);
        map<int, double> FillMapWithZeros();

        double area_and_line(vector<double>& coordImpulses);

        std::string impulse_hist_and_stand(vector<double>& x);

        std::string FindMaxIntersection(double areaWater, double areaForest);//, double areaField, double areaPlowedField);
    };
}
