#ifndef DIALOGSHOWATTRADIUS_H
#define DIALOGSHOWATTRADIUS_H

#include <QDialog>

namespace Ui {
class DialogShowAttradius;
}

class DialogShowAttradius : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogShowAttradius(QWidget *parent = 0);
    ~DialogShowAttradius();
    
private slots:
    void on_pushButtonClose_clicked();

public slots:
    void slotShowAttradius(QString *str);

private:
    Ui::DialogShowAttradius *ui;
};

#endif // DIALOGSHOWATTRADIUS_H
