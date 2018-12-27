#include <QRubberBand>
#include "customplotzoom.h"
#include <QLinearGradient>
CustomPlotZoom::CustomPlotZoom(QWidget * parent)
    : QCustomPlot(parent)
    , mZoomMode(false)
    , mXZoomMode(false)
    , mSignalMode(false)
    , x1(0)
    , x2(x())
    , mRubberBand(new QRubberBand(QRubberBand::Rectangle, this))
    ,rubberheight(178)
    ,IsPSD(false)
    ,IsDemoduling(false)
{
    design();
    rubberheight=this->height()-30;


}

CustomPlotZoom::~CustomPlotZoom()
{
    delete mRubberBand;
}

void CustomPlotZoom::setZoomMode(bool mode)
{
    mZoomMode = mode;
}

void CustomPlotZoom::setXZoomMode(bool mode)
{
    mXZoomMode = mode;
}

void CustomPlotZoom::setSignalMode(bool mode)
{
    mSignalMode = mode;
}

void CustomPlotZoom::Rescale()
{
    if(abs(x2-x1)<0.1)
        x2=0;
    if(x2>0)
    {
        xAxis->setRange(x1, x2);

    }
}
void CustomPlotZoom::resizeEvent(QResizeEvent *event)
{
    rubberheight=this->height()-30;
    QCustomPlot::resizeEvent(event);
}

void CustomPlotZoom::Setfftlen(double v)
{
    fftlen=v;qDebug()<<v<<"costumplotSetfftlen";
}

void CustomPlotZoom::On_Reset()
{


    hiderubbers();
    hiderubbersDemod();
}

void CustomPlotZoom::mousePressEvent(QMouseEvent * event)
{
    if(!IsPSD || IsDemoduling)
        return;
    if (mZoomMode)
    {
        if (event->button() == Qt::RightButton)
        {
            hiderubbers();
            mOrigin = event->pos();
            if (mXZoomMode)
                mOrigin.setY(0);
            mRubberBand->setGeometry(QRect(mOrigin, QSize()));
            mRubberBand->show();
        }
        if (event->button() == Qt::LeftButton)
        {

            mOrigin = event->pos();
            mOrigin.setY(0);
            mRubberBand->setGeometry(QRect(mOrigin, QSize()));
            mRubberBand->show();
        }
    }
    QCustomPlot::mousePressEvent(event);
}

void CustomPlotZoom::mouseMoveEvent(QMouseEvent * event)
{   

    if(IsPSD)
    {

        double   x1_ = xAxis->pixelToCoord(event->pos().x());


        double xx1=x1_*((double)(200000000/2))/(fftlen/2);
        xx1*=1000;
        this->setToolTip(QString("%1 MHZ").arg(x1_));
    }

    if(!IsPSD || IsDemoduling)
        return;
    if (mRubberBand->isVisible() )
    {
        if (mXZoomMode)
        {
            mRubberBand->setGeometry(QRect(mOrigin, QPoint(event->x(),rubberheight)).normalized());
            mRubberBand->show();
        }
        else
        {
            mRubberBand->setGeometry(QRect(mOrigin, event->pos()).normalized());
        }



    }
    QCustomPlot::mouseMoveEvent(event);

}

void CustomPlotZoom::mouseReleaseEvent(QMouseEvent * event)
{
     replot();
    if(!IsPSD || IsDemoduling)
        return;
    if (mRubberBand->isVisible())
    {



        if(event->button() == Qt::RightButton)
        {

            const QRect & zoomRect = mRubberBand->geometry();
            int xp1, yp1, xp2, yp2;
            zoomRect.getCoords(&xp1, &yp1, &xp2, &yp2);
            x1 = xAxis->pixelToCoord(xp1);
            x2 = xAxis->pixelToCoord(xp2);
            double y1 = yAxis->pixelToCoord(yp1);
            double y2 = yAxis->pixelToCoord(yp2);
            x1out=x1;
            x2out=x2;
            //qDebug() << x1 << x2 << y1 << y2 << "========================";
           // qDebug() << xp1 << xp2 << "========================";




            if(std::abs(xp1-xp2)<=1)
            {
                rescaleAxes();

            }
            else{
                if (mSignalMode)
                {
                    emit posZoom(x1,y1,x2,y2);

                }
                else
                {

                    xAxis->setRange(x1, x2);
                    if (!mXZoomMode)
                    {
                        yAxis->setRange(y1, y2);

                    }
                }
            }
            replot();

        }
        else
        {
            if(abs(mOrigin.x()-event->pos().x())>5)
            {
                QRubberBand *QRB=new QRubberBand(QRubberBand::Rectangle, this);
                QRB->setGeometry(QRect(mOrigin, QPoint(event->x(),rubberheight)).normalized());
                QRB->show();
                rubberlist.append(QRB);



                const QRect & zoomRect = QRB->geometry();
                int xp1, yp1, xp2, yp2;
                zoomRect.getCoords(&xp1, &yp1, &xp2, &yp2);
                double   x1_ = xAxis->pixelToCoord(xp1);
                double  x2_ = xAxis->pixelToCoord(xp2);
                double xx1=x1_;
                double xx2=x2_;
                xx1*=1000000;
                xx2*=1000000;
                x1out=xx1;
                x2out=xx2;
                //            double xx1=x1_*125000.0/(fftlen/2);
                //            double xx2=x2_*125000.0/(fftlen/2);
                //            xx1*=1000;
                //            xx2*=1000;
                if(!IsDemoduling){
                    emit SelectedForAMR(xx1,xx2);
                    emit GetForFCEstimate();
                }

            }

        }

        mRubberBand->hide();
    }
    QCustomPlot::mouseReleaseEvent(event);
}

void CustomPlotZoom::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(!IsPSD)
        return;
    if(event->button() == Qt::RightButton)
    {
        x2=0;
    }
    else
    {
        hiderubbers();
    }
    QCustomPlot::mouseDoubleClickEvent(event);

}

void CustomPlotZoom::design()
{

    //    set some pens, brushes and backgrounds:

    //    xAxis->setBasePen(QPen(Qt::white, 1));
    //    yAxis->setBasePen(QPen(Qt::white, 1));
    //    xAxis->setTickPen(QPen(Qt::white, 1));
    //    yAxis->setTickPen(QPen(Qt::white, 1));
    //    xAxis->setSubTickPen(QPen(Qt::white, 1));
    //    yAxis->setSubTickPen(QPen(Qt::white, 1));
    //    xAxis->setTickLabelColor(Qt::white);
    //    yAxis->setTickLabelColor(Qt::white);
    //    xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    //    yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    //    xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    //    yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    //    xAxis->grid()->setSubGridVisible(true);
    //    yAxis->grid()->setSubGridVisible(true);
    //    xAxis->grid()->setZeroLinePen(Qt::NoPen);
    //    yAxis->grid()->setZeroLinePen(Qt::NoPen);
    //    xAxis->setUpperEnding(QCPLineEnding::esNone);
    //    yAxis->setUpperEnding(QCPLineEnding::esNone);
    QLinearGradient plotGradient;
    plotGradient.setStart(1, 0);
    plotGradient.setFinalStop(0, 350);
    plotGradient.setColorAt(0, QColor(80, 80, 80));
    plotGradient.setColorAt(1, QColor(50, 50, 50));
    setBackground(plotGradient);

    //    yAxis->setRange(0, 2);




    //    xAxis2->setVisible(false);
    //    xAxis2->setTickLabels(false);
    //    yAxis2->setVisible(false);
    //    yAxis2->setTickLabels(false);





    //   xAxis2->setVisible(true);
    //   xAxis2->setTickLabels(false);
    //   yAxis2->setVisible(true);
    //   yAxis2->setTickLabels(false);



}

void CustomPlotZoom::hiderubbers()
{
    if(!IsPSD || rubberlist.count()==0)
        return;
    for(int i=0;i<rubberlist.count();i++)
    {
        rubberlist[i]->hide();
    }
    rubberlist.clear();



}

void CustomPlotZoom::hiderubbersDemod()
{
    if(!IsPSD || Demodrubberlist.count()==0)
        return;
    for(int i=0;i<Demodrubberlist.count();i++)
    {
        Demodrubberlist[i]->hide();
    }
    Demodrubberlist.clear();
    Demclicked.clear();

    int count=textItems.count();
    for(int i=0;i<count;i++)
    {
        textItems[i]->setVisible(false);
    }
}

void CustomPlotZoom::AddDemodRubberList(double fc, int DemNum)
{

    bool flag=false;

    for(int i=0;i<Demclicked.count();i++)
    {
        if(Demclicked[i]==DemNum)
        {
            flag=true;
            break;
        }
    }
    if(flag)
        return;
    Demclicked.append(DemNum);
    QPoint p1;
    p1.setX(xAxis->coordToPixel((fc/1000000)-1));
    p1.setY(0);
    QPoint p2;
    p2.setX(xAxis->coordToPixel((fc/1000000)+1));
    p2.setY(rubberheight-50);

    // add label for group tracer:

    int count=textItems.count();
  flag=false;
    for(int i=0;i<count;i++)
    {
        if("Dem "+QString::number(DemNum)==textItems[i]->text())
        {
            flag=true;
            textItems[i]->position->setCoords(p1.x()+12,rubberheight-23); // lower right corner of axis rect
            textItems[i]->setVisible(true);
            break;
        }
    }
    if(!flag)
    {
        QCPItemText *groupTracerText = new QCPItemText(this);
        groupTracerText->position->setType(QCPItemPosition::ptAbsolute);
        groupTracerText->position->setCoords(p1.x()+12,rubberheight-23); // lower right corner of axis rect
        groupTracerText->setRotation(-90);
        groupTracerText->setText("Dem "+QString::number(DemNum));
        groupTracerText->setFont(QFont(font().family(),11));
        textItems.append(groupTracerText);
    }

    //////////////////////////////////////////////////////

    hiderubbers();
    QRubberBand *QRB=new QRubberBand(QRubberBand::Rectangle, this);
    QRB->setGeometry(QRect(p1, p2));
    QRB->show();
    Demodrubberlist.append(QRB);
    QPalette pal;
    QColor color(Qt::red);

    switch (DemNum) {
    case 1:
        color.setRed(255);
        color.setGreen(0);
        color.setBlue(0);
        break;
    case 2:
        color.setRed(0);
        color.setGreen(255);
        color.setBlue(0);
        break;
    case 3:
        color.setRed(0);
        color.setGreen(0);
        color.setBlue(255);
        break;
    case 4:
        color.setRed(255);
        color.setGreen(255);
        color.setBlue(255);
        break;
    }
    color.setAlpha(10);
    pal.setBrush(QPalette::Highlight,QBrush(color));
    QRB->setPalette(pal);
    /////////////////







}

void CustomPlotZoom::AddName(QString name)
{

//        QCPItemText *groupTracerText = new QCPItemText(this);
//        groupTracerText->position->setType(QCPItemPosition::ptAbsolute);
//        groupTracerText->position->setCoords(90,20); // lower right corner of axis rect
//        groupTracerText->setText(name);
//        groupTracerText->setFont(QFont(font().family(),11));
//        groupTracerText->setVisible(true);
       // setWindowTitle(name);
        QCPPlotTitle *pt=new QCPPlotTitle(this,name);

}

void CustomPlotZoom::ShowDemInfo(bool Cover, bool Lable)
{
    if(!Cover)
    {
        for(int i=0;i<Demodrubberlist.count();i++)
        {
            Demodrubberlist[i]->hide();
        }

    }
    else
    {
        for(int i=0;i<Demodrubberlist.count();i++)
        {
            Demodrubberlist[i]->show();
        }
    }
    if(!Lable)
    {
        int count=textItems.count();
        for(int i=0;i<count;i++)
        {
            textItems[i]->setVisible(false);

        }
    }
    else
    {
        int count=textItems.count();
        for(int i=0;i<count;i++)
        {
            textItems[i]->setVisible(true);
        }
    }
}
