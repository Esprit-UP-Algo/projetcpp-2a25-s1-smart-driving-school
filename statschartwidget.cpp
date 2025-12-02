#include "statschartwidget.h"
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QPainterPath>
#include <algorithm>

StatsChartWidget::StatsChartWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumHeight(220);
    setAutoFillBackground(true);
}

void StatsChartWidget::setData(const QVector<double>& monthlyPercent, const QString& title)
{
    data_  = monthlyPercent;
    title_ = title;
    update();
}

void StatsChartWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    // Fond
    p.fillRect(rect(), palette().base());

    // Marges du graphique
    const int left   = 50;
    const int right  = 20;
    const int top    = 35;
    const int bottom = 35;
    QRect plot(left, top, width() - left - right, height() - top - bottom);

    // Cadre + titre
    p.setPen(QPen(Qt::black, 1));
    p.drawRect(plot.adjusted(-1,-1,1,1));
    if (!title_.isEmpty()) {
        QFont f = p.font(); f.setBold(true);
        p.setFont(f);
        p.drawText(QRect(0, 5, width(), 20), Qt::AlignCenter, title_);
    }

    // Axes Y (0..100)
    p.setFont(QFont(p.font().family(), 9));
    for (int y=0; y<=5; ++y) {
        const double val = y * 20.0; // 0,20,...100
        int yy = plot.bottom() - int(plot.height() * (val/100.0));
        p.setPen(QPen(Qt::lightGray, 1, Qt::DotLine));
        p.drawLine(plot.left(), yy, plot.right(), yy);
        p.setPen(QPen(Qt::black, 1));
        p.drawText(5, yy-6, 40, 12, Qt::AlignRight|Qt::AlignVCenter, QString::number(int(val)));
    }

    // Labels X (mois)
    const QStringList mois = {"Jan","Fév","Mar","Avr","Mai","Jun","Jul","Aoû","Sep","Oct","Nov","Déc"};
    const int n = 12;
    const double stepX = n>1 ? (plot.width() * 1.0 / (n-1)) : plot.width();
    p.setPen(Qt::black);
    for (int i=0; i<n; ++i) {
        int x = plot.left() + int(i*stepX);
        p.drawText(x-15, plot.bottom()+5, 30, 14, Qt::AlignHCenter|Qt::AlignTop, mois[i]);
    }

    if (data_.size() != 12) return;

    // Courbe
    QPainterPath path;
    for (int i=0; i<n; ++i) {
        double v = std::clamp(data_[i], 0.0, 100.0);
        int x = plot.left() + int(i*stepX);
        int y = plot.bottom() - int(plot.height() * (v/100.0));
        if (i==0) path.moveTo(x,y);
        else      path.lineTo(x,y);
    }
    p.setPen(QPen(Qt::darkCyan, 2));
    p.drawPath(path);

    // Points
    p.setBrush(QBrush(Qt::darkCyan));
    for (int i=0; i<n; ++i) {
        double v = std::clamp(data_[i], 0.0, 100.0);
        int x = plot.left() + int(i*stepX);
        int y = plot.bottom() - int(plot.height() * (v/100.0));
        p.drawEllipse(QPoint(x,y), 3,3);
    }
}
