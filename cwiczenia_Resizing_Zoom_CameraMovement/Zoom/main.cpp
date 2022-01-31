#include "app.h"

int main()
{

    SimpleShapeApplication app(650*2, 480*2, PROJECT_NAME, true);
    app.run(1);

    return 0;
}