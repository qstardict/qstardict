#include <KStatusNotifierItem>
#include <KWindowSystem>
#include <KGlobalAccel>
#include <QApplication>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    new KStatusNotifierItem("qstardict", 0);
}
