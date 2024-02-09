#include <cmath>
#include <matplot/matplot.h>
#include <algorithm>
#include <vector>
#include <random>
#include <numeric>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;
//using namespace matplot;

namespace app_graphics
{
    using namespace std;
    using namespace matplot;

    class Graphics {
    private:
    public:
        std::vector<double> Times;
        std::vector<double> Alts;
        std::vector<double> Impulses;
        std::string AltGraphPath;
        std::string ImpulseGraphPath;

        Graphics(std::vector<double>& times, std::vector<double>& alts, std::vector<double>& impulses);
        Graphics();
        std::string GetGraphics();

        std::vector<double> x11, x21, y11, y21;
        vector<vector<double>> impulse;
        vector<double> imps, max_imps, water, forest, field, plowed_field;
        std::string decision;

        void alt_graph(std::vector<double> alt, std::vector<double> time);
        void impulse_graph(std::vector<double> x);
        void impulse_hist(std::vector<double> x);
        std::vector<double> read_vector(std::vector<double> surface, std::string file);
        map<int, double> zero_map();

        double area_and_line(vector<double> x, vector<double> y);

        std::string impulse_hist_and_stand(vector<double> x);

        std::string findMax(double areaWater, double areaForest, double areaField, double areaPlowedField);
    };
}
