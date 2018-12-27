#pragma once
 
#include <QPoint>
#include "qcustomplot.h"
#include <cmath>
 #include <QPalette>

class QRubberBand;
class QMouseEvent;
class QWidget;

class CustomPlotZoom : public QCustomPlot
{
    Q_OBJECT

public:
    CustomPlotZoom(QWidget * parent = 0);
    virtual ~CustomPlotZoom();

    void setZoomMode(bool mode);
    void setXZoomMode(bool mode);
    void setSignalMode(bool mode);
    void Rescale();
    void AddDemodRubberList(double fc,int DemNum);
      void AddName(QString name);
    bool IsPSD;
    bool IsDemoduling;
    double fftlen;
    void ShowDemInfo(bool Cover,bool Lable);


signals:
    void posZoom(double x1,double y1,double x2,double y2);
    void SelectedForAMR(double x1,double x2);
    void GetForFCEstimate();

private slots:
    void mousePressEvent(QMouseEvent * event) override;
    void mouseMoveEvent(QMouseEvent * event) override;
    void mouseReleaseEvent(QMouseEvent * event) override;
    void mouseDoubleClickEvent(QMouseEvent * event) override;
    void resizeEvent(QResizeEvent *event);
    void Setfftlen(double v);
    void On_Reset();

private:
    void hiderubbers();
        void hiderubbersDemod();
    void design();
    bool mZoomMode;
    bool mXZoomMode;
    bool mSignalMode;
    QRubberBand * mRubberBand;
    QPoint mOrigin;
    double x1;
    double x2;
    QList<QRubberBand*> rubberlist;
    QList<QRubberBand*> Demodrubberlist;
    QList<QCPItemText*> textItems;
    QList<int> Demclicked;
    int rubberheight;
public:
   double x1out;
   double x2out;


};
