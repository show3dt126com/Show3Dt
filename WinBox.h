#ifndef BOTTOMWINDOWS_H
#define BOTTOMWINDOWS_H

#include <QMainWindow>
#include <QObject>
#include <QDockWidget>

class WinBox : public QMainWindow
{
    Q_OBJECT
public:
    explicit WinBox(QWidget *parent = nullptr, QString caption="");
    void addWindow(QWidget * w);
    void addWindow(QWidget * w, QString title);
    void removeAllWindows();
    void arrangeWindows();

    int windowNum;
    QDockWidget dockWindows[20];
    bool docked[20];

    void doAddDockWidget(QWidget * w);
    // Qt::Horizontal Qt::Vertical
    void doSplitDockWidget(QWidget *after, QWidget *w, Qt::Orientation orient);
    void doTabifyDockWidget(QDockWidget *first, QDockWidget *second);
    //by sxd ¼üÅÌÊÂ¼þ
    virtual void focusInEvent(QFocusEvent *e) override;
    virtual void focusOutEvent(QFocusEvent *e) override;
signals:

public slots:
    void onAddData(QString data);
};

#endif // BOTTOMWINDOWS_H
