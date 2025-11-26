#ifndef STATSCHARTWIDGET_H
#define STATSCHARTWIDGET_H

#include <QWidget>
#include <QVector>

class StatsChartWidget : public QWidget {
    Q_OBJECT
public:
    explicit StatsChartWidget(QWidget *parent = nullptr);
    void setData(const QVector<double>& monthlyPercent, const QString& title);

protected:
    void paintEvent(QPaintEvent *) override;

private:
    QVector<double> data_;   // 12 valeurs (0..100)
    QString title_;
};

#endif // STATSCHARTWIDGET_H
