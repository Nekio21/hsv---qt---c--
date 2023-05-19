#include "ekran.h"

#include <QPainter>
#include <QMouseEvent>
#include <QLabel>
#include <cmath>
#include<iostream>
#include <algorithm>

using namespace std;

Ekran::Ekran(QWidget *parent)
    : QWidget{parent}
{
    im = QImage(1200, 1000, QImage::Format_RGB32);
    im.fill(0);
    im.load("C:\\Users\\user\\Pictures\\Camera Roll\\zdj.png");

    imCopy = QImage(1200, 1000, QImage::Format_RGB32);
    imCopy.fill(0);
    imCopy.load("C:\\Users\\user\\Pictures\\Camera Roll\\zdj.png");


    srand( time(NULL));

}

void Ekran::paintEvent(QPaintEvent *){
    QPainter p(this);
    p.fillRect(0,0, width(), height(), Qt::green);
    p.drawImage(0,0, im);
    p.drawImage(0,0, imCopy);
}

void Ekran::drawPixel(int x, int y, int r, int g, int b){

    if((x<0) || (y<0) || (im.width()<=x) || (im.height()<=y)) return;

    uchar *pix = im.scanLine(y) + 4*x;
    pix[0] = b;
    pix[1] = g;
    pix[2] = r;

}

void Ekran::drawPixel(int x, int y, int r, int g, int b, QImage* im){

    if((x<0) || (y<0) || (im->width()<=x) || (im->height()<=y)) return;

    uchar *pix = im->scanLine(y) + 4*x;
    pix[0] = b;
    pix[1] = g;
    pix[2] = r;

}

void Ekran::drawPixel(int x, int y, Rgb rgb){

   drawPixel(x, y, rgb.r, rgb.g, rgb.b);

}

void Ekran::drawPixel(int x, int y, Rgb rgb, QImage* im){

   drawPixel(x, y, rgb.r, rgb.g, rgb.b, im);

}

Rgb Ekran::getColor(int x, int y){

    if((x<0) || (y<0) || (im.width()<=x) || (im.height()<=y)) return {-1,-1,-1};

    uchar *pix = im.scanLine(y) + 4*x;

    return {pix[2], pix[1], pix[0]};
}

void Ekran::wybierzPunkt(int x, int y){
    int linia;
    int liniaMin = width();

    for(int i=0;i<ve.size(); i++){
        linia = countR(x,y, ve[i].x, ve[i].y);

        if((linia <= 5)&&(liniaMin > linia)){
            indexOptionD = i;
            liniaMin = linia;
            cout<<"wybrano index: "<<indexOptionD<<endl;
        }
    }
}

void Ekran::krzywaBezieraZaladuj(){
    im.fill(0);

    int ilosc = ve.size()-1;

    bool pokazKropki;

    if(option!='c'){
        pokazKropki = true;
    }

    for(int i=0;i<(ilosc/3);i++){
        rysujKrzywaBeziera(ve[i*3], ve[i*3+1], ve[i*3+2], ve[i*3+3],50, pokazKropki);
    }

    update();
}

void Ekran::bsplineZaladuj(){
    im.fill(0);

    int ilosc = ve.size()-1;

    bool pokazKropki;

    if(option!='c'){
        pokazKropki = true;
    }


    if(ilosc >= 3){
        for(int i=0;i<ilosc-2;i++){
            rysujBspline(ve[i], ve[i+1], ve[i+2], ve[i+3],50, pokazKropki);
        }
    }

    update();
}

int Ekran::zwrocY(int x, int x1, int y1, int x2, int y2){
    int y;

    y = y1 + (x-x1)*(double)(((y2-y1)/(double)(x2-x1))) + 0.5;

    return y;
}

int Ekran::zwrocX(int y, int x1, int y1, int x2, int y2){
    int x;

    x = ( (double)(y-y1) / ((double)(y2-y1)/(x2-x1)) ) + x1 + 0.5;



    return x;
}


int Ekran::countR(int x1, int y1, int x2, int y2){
   int value1 = (x2-x1) * (x2-x1);
   int value2 = (y2-y1) * (y2-y1);

   int suma = value1 + value2;

   int R = sqrt(suma);

   return R;
}

void Ekran::rysujOkrag(int x0, int y0, int R){

    for(int x=0;;x++){

        int y = sqrt(R*R-x*x)+0.5;
        if(x>y)break;


        drawPixel(x+x0,y+y0,134,144,22);
        drawPixel(y+x0,x+y0,134,144,22);
        drawPixel(-x+x0,-y+y0,134,144,22);
        drawPixel(-y+x0,-x+y0,134,144,22);

        drawPixel(-x+x0,y+y0,134,144,22);
        drawPixel(-y+x0,x+y0,134,144,22);
        drawPixel(x+x0,-y+y0,134,144,22);
        drawPixel(y+x0,-x+y0,134,144,22);
    }

}


void Ekran::rysujElipse(int x0,int  y0,int R1,int R2,int N){

    int x, xOld;
    int y, yOld;

    xOld = R1 * cos(0);
    yOld = R2 * sin(0);


    for(int i=1;i<=N;++i){
        double alfa = (2*M_PI*i)/(double)N;

        x = R1 * cos(alfa);
        y = R2 * sin(alfa);


        rysujLinie(x+x0, y+y0, xOld+x0, yOld+y0);

        xOld = x;
        yOld = y;
    }
}



void Ekran::rysujKrzywaBeziera(Punkt p1, Punkt p2, Punkt p3, Punkt p4, int iloscLini, bool show){

    int xOld = p1.x;
    int yOld = p1.y;

    if(show){
        rysujOkrag(p1.x, p1.y,4);
        rysujOkrag(p2.x, p2.y,4);
        rysujOkrag(p3.x, p3.y,4);
        rysujOkrag(p4.x, p4.y,4);
    }


    for(int k=1;k<=iloscLini;k++){

        int x = krzywaBeziera(p1.x, p2.x, p3.x, p4.x, k/(float)iloscLini);
        int y = krzywaBeziera(p1.y, p2.y, p3.y, p4.y, k/(float)iloscLini);

        rysujLinie(xOld, yOld, x, y);

        xOld = x;
        yOld = y;
    }
}


void Ekran::rysujBspline(Punkt p1, Punkt p2, Punkt p3, Punkt p4, int iloscLini, bool show){

    int xOld = krzywaBspline(p1.x, p2.x, p3.x, p4.x, 0);
    int yOld = krzywaBspline(p1.y, p2.y, p3.y, p4.y, 0);


    if(show){
        rysujOkrag(p1.x, p1.y,4);
        rysujOkrag(p2.x, p2.y,4);
        rysujOkrag(p3.x, p3.y,4);
        rysujOkrag(p4.x, p4.y,4);
    }

    for(int k=1;k<=iloscLini;k++){

        int x = krzywaBspline(p1.x, p2.x, p3.x, p4.x, k/(float)iloscLini);
        int y = krzywaBspline(p1.y, p2.y, p3.y, p4.y, k/(float)iloscLini);

        rysujLinie(xOld, yOld, x, y);

        xOld = x;
        yOld = y;
    }

}


//jesli chcesz wynik krzywej dla y wpisz y, a jesli dla x wpisz x;
//czym jest t??? T jest jakby ulamkniem
//jesli chcemy znac paremetry punktu w 1/2 w krzywej to t=0.5
//jesli na koncu to t = 1
//jesli na poczatku to t= 0
int Ekran::krzywaBeziera(int p1,int p2, int p3, int p4, double t){
    int wynik = 0;

    double wynikPart1 = (1-t)*(1-t)*(1-t) * p1;
    double wynikPart2 = 3 * (1-t)*(1-t)*t * p2;
    double wynikPart3 = 3 * (1-t)*t*t * p3;
    double wynikPart4 = t*t*t * p4;

    wynik = wynikPart1 + wynikPart2 + wynikPart3 + wynikPart4;

    return wynik;
}


int Ekran::krzywaBspline(int p1,int p2, int p3, int p4, double t){
    int wynik = 0;

    double wynikPart1 = (-t*t*t + 3*t*t -3*t + 1)/(double)6 * p4;
    double wynikPart2 = (3*t*t*t-6*t*t+4)/(double)6 * p3;
    double wynikPart3 = (-3*t*t*t+3*t*t+3*t+1)/(double)6 * p2;
    double wynikPart4 = t*t*t/(double)6 * p1;

    wynik = wynikPart1 + wynikPart2 + wynikPart3 + wynikPart4;

    return wynik;
}


void Ekran::rysujLinie(int x1, int y1, int x2, int y2){
    if(abs(y2-y1) <= abs(x2-x1)){

        if(x2 > x1){
         for(int x = x1;x<x2;x++){
             int y = zwrocY(x, x1, y1, x2, y2);
             drawPixel(x+0.5,y+0.5, chooseRgb);
         }
        }else{
            for(int x = x1;x>x2;x--){
                int y = zwrocY(x, x1, y1, x2, y2) ;
                drawPixel(x+0.5,y, chooseRgb);
            }
        }
     }else{
        if(y2 > y1){
            for(int y = y1;y<y2;y++){
                int x = zwrocX(y, x1, y1, x2, y2);
                drawPixel(x+0.5,y+0.5, chooseRgb);
            }
        }else{
            for(int y = y1;y>y2;y--){
                int x = zwrocX(y, x1, y1, x2, y2);
                drawPixel(x+0.5,y+0.5, chooseRgb);
            }
        }

    }
}

//zamaluj bierze z zamalujPunkty(taki vector) pukty do obrowbki
//wiec nalezy przed wywolaniem metody wywolac push back zamalujPunkty :)
void Ekran::zamaluj(Rgb color, Rgb background){

    if((color.r == background.r) &&
            (color.g == background.g) &&
            (color.b == background.b)){

        zamalujPunkty.pop_back();
        return;
    }

    while(!zamalujPunkty.empty()){
        Punkt punkt = zamalujPunkty.back();
        zamalujPunkty.pop_back();

        if((getColor(punkt.x,punkt.y).r == background.r) &&
            (getColor(punkt.x,punkt.y).g == background.g) &&
            (getColor(punkt.x,punkt.y).b == background.b)){

            drawPixel(punkt.x, punkt.y, chooseRgb);

            zamalujPunkty.push_back({punkt.x+1,punkt.y});
            zamalujPunkty.push_back({punkt.x-1,punkt.y});
            zamalujPunkty.push_back({punkt.x,punkt.y+1});
            zamalujPunkty.push_back({punkt.x,punkt.y-1});
        }
    }

    update();
}


void Ekran::rysujPoligon(vector<Punkt> ve){
    int minY = (*min_element(ve.begin(), ve.end(),[](Punkt a,Punkt b){return a.y<b.y;})).y;
    int maxY = (*max_element(ve.begin(), ve.end(),[](Punkt a,Punkt b){return a.y<b.y;})).y;

    vector<int> output;

    cout<<minY<<", "<<maxY<<endl;

    for(int y=minY; y<=maxY; y++){

        cout<<"siema"<<endl;
        findX(output, ve, y);

        sort(output.begin(), output.end());

        for(int i=0;i<(int)output.size();i++){
            cout<<"Pozycja nr "<<i<<": "<<output[i]<<endl;
        }

        wypelnij(output, y);
    }
}


void Ekran::wypelnij(vector<int> x, int y){

    for(int i=1; i<x.size();i++){

        if(i%2==0)continue;

        rysujLinie(x[i-1], y, x[i], y);
    }
}

void Ekran::findX(vector<int>& output, vector<Punkt> ve, int y){
    int k=0;

    output.clear();

    for(int i=0; i<(int)ve.size(); i++){
        int y1 = ve[i].y;
        int y2 = ve[(i+1)%ve.size()].y;

        if((y1>=y && y2<y) || (y2>=y && y1<y)){
            int x = findXDeep(ve[i],ve[(i+1)%ve.size()], y);

            cout<<"dodajem"<<endl;
            output.push_back(x);
        }
    }
}

int Ekran::findXDeep(Punkt p1, Punkt p2, int y){
   int returnValue;
   int returnValuePart1;
   int returnValuePart2;
   int returnValuePart3;

   //if(p1.y == p2.y) return

   returnValuePart1 = p1.x;
   returnValuePart2 = (p2.x - p1.x) * (y-p1.y);
   returnValuePart3 = p2.y-p1.y;

   returnValue = returnValuePart1 + returnValuePart2/(double)returnValuePart3;

   return returnValue;
}

//type=1 => H
//type=2 => S
//type=3 => V
void Ekran::updateHSV(){


    for(int y=0;y<im.height();y++){
        for(int x=0;x<im.width();x++){
            Rgb rgb = getColor(x,y);
            HSV hsvTemplate = getHSVColor(rgb);

            //cout<<"-------------"<<endl;
            //cout<<"RGB: {"<<rgb.r<<", "<<rgb.g<<", "<<rgb.b<<"}"<<endl;
            //cout<<"HSV: {"<<hsvTemplate.h<<", "<<hsvTemplate.s<<", "<<hsvTemplate.v<<"}"<<endl;

            hsvTemplate.h = max(0, min(hsvTemplate.h + hsv.h, 360));
            hsvTemplate.s = max(0.0, min(hsvTemplate.s + hsv.s, 1.0));
            hsvTemplate.v = max(0.0, min(hsvTemplate.v + hsv.v, 1.0));

            //cout<<"HSV_NEW: {"<<hsvTemplate.h<<", "<<hsvTemplate.s<<", "<<hsvTemplate.v<<"}"<<endl;

            Rgb newRgb = getRgbColor(hsvTemplate);

           // cout<<"RGB_NEW: {"<<newRgb.r<<", "<<newRgb.g<<", "<<newRgb.b<<"}"<<endl;
            //cout<<"-------------"<<x<<","<<y<<endl;

            drawPixel(x,y, newRgb, &imCopy);
        }
    }

    update();
}

Rgb Ekran::getRgbColor(HSV hsv){
    double c = hsv.v * hsv.s;
    double x = c * (1-abs((fmod ((hsv.h/60.0),2)-1)));
    double m = hsv.v - c;

    RgbT rgbT;

    int h = hsv.h;

    if(0<= h && h < 60){
        rgbT.r = c;
        rgbT.g = x;
        rgbT.b = 0;
    }else if(60<= h && h < 120){
        rgbT.r = x;
        rgbT.g = c;
        rgbT.b = 0;
    }else if(120<=h && h<180){
        rgbT.r = 0;
        rgbT.g = c;
        rgbT.b = x;
    }else if(180<=h && h<240){
        rgbT.r = 0;
        rgbT.g = x;
        rgbT.b = c;
    }else if(240<=h && h<300){
        rgbT.r = x;
        rgbT.g = 0;
        rgbT.b = c;
    }else if(300<=h && h<=360){
        rgbT.r = c;
        rgbT.g = 0;
        rgbT.b = x;
    }

    Rgb returnRgb;

    returnRgb.r = (rgbT.r+m)*255;
    returnRgb.g = (rgbT.g+m)*255;
    returnRgb.b = (rgbT.b+m)*255;

    return returnRgb;
}

HSV Ekran::getHSVColor(Rgb rgb){
    double r2 = rgb.r/255.0;
    double g2 = rgb.g/255.0;
    double b2 = rgb.b/255.0;

    double valueMax = max(r2, max(g2,b2));
    double valueMin = min(r2, min(g2,b2));

    double delta = valueMax - valueMin;

    HSV returnHSV;


    returnHSV.h = getCountH(delta, valueMax, r2,g2,b2);
    returnHSV.s = getCountS(delta, valueMax);
    returnHSV.v = valueMax;

    return returnHSV;
}


int Ekran::getCountH(double delta, double max, double r2, double g2, double b2){
    if(delta == 0){
        return 0;
    }else if(max == r2){
        return (60*(fmod(((g2-b2)/delta),6)));
    }else if(max == g2){
         return (60*(((b2-r2)/delta)+2));
    }else if(max == b2){
         return (60*(((r2-g2)/delta)+4));
    }
}

double Ekran::getCountS(double delta, double max){
    if(max == 0) return 0;
    else return delta/max;
}
