#ifndef EKRAN_H
#define EKRAN_H

#include <QWidget>
#include <vector>
#include <iostream>

using namespace std;

struct Punkt{
    int x;
    int y;
};

struct Rgb{
    int r;
    int g;
    int b;
};

struct RgbT{
    double r;
    double g;
    double b;
};

struct HSV{
    int h;
    double s;
    double v;
};

class Ekran : public QWidget
{
    Q_OBJECT
public:
    explicit Ekran(QWidget *parent = nullptr);
    void krzywaBezieraZaladuj();
    void bsplineZaladuj();
    char option = 'p';
    Rgb chooseRgb = {124,54,23};
    HSV hsv = {0,0,0};
    void updateHSV();
protected:
    void paintEvent(QPaintEvent *);

private:
    QImage im;
    QImage imCopy;
    vector<Punkt> ve;
    vector<Punkt> zamalujPunkty;
    vector<Punkt> polygonPoints;
    int indexOptionD;

    void drawPixel(int x, int y, int r, int g, int b);
    void drawPixel(int x, int y, int r, int g, int b, QImage* im);
    void drawPixel(int x, int y, Rgb rgb, QImage* im);
    void drawPixel(int x, int y, Rgb rgb);

    Rgb getColor(int x, int y);
    int zwrocY(int x, int x1, int y1, int x2, int y2);
    int zwrocX(int y, int x1, int y1, int x2, int y2);
    void zamaluj(Rgb, Rgb);
    void rysujOkrag(int,int,int);
    void rysujElipse(int x0, int y0, int R1, int R2, int N);
    void rysujLinie(int x1, int y1, int x2, int y2);
    void rysujKrzywaBeziera(Punkt,Punkt,Punkt, Punkt, int, bool);
    void rysujBspline(Punkt,Punkt,Punkt, Punkt, int, bool);
    int krzywaBeziera(int,int,int,int, double);
    int krzywaBspline(int,int,int,int, double);
    void rysujPoligon(vector<Punkt>);
    void findX(vector<int>&, vector<Punkt>, int y);
    int findXDeep(Punkt, Punkt, int y);
    void wybierzPunkt(int, int);
    int countR(int,int,int,int);
    void wypelnij(vector<int> x, int y);
    double getCountS(double delta, double max);
    int getCountH(double delta, double max, double r2, double g2, double b2);
    HSV getHSVColor(Rgb rgb);
    Rgb getRgbColor(HSV hsv);
    int x1;
    int y1;
    int x2;
    int y2;
    int xOldLinia = -1;
    int yOldLinia = -1;
    bool second = false;
signals:

};

#endif // EKRAN_H
