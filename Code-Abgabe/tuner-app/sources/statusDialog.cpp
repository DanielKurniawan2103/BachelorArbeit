#include "statusDialog.h"
#include <QLabel>
#include <QVBoxLayout>

StatusDialog::StatusDialog(QWidget *parent) : QWidget(parent) {
    setWindowTitle("Verbindungsstatus");
    resize(400, 250);

    // Setze den Hintergrund auf schwarz
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, Qt::black);
    this->setPalette(palette);

    QVBoxLayout *layout = new QVBoxLayout(this);

    // Erstelle ein Label mit weißer Schrift und großer Schriftgröße
    QLabel *label = new QLabel("Verbindung zum Tuner wird aufgebaut...", this);
    QPalette labelPalette = label->palette();
    labelPalette.setColor(QPalette::WindowText, Qt::white);
    label->setPalette(labelPalette);
    label->setAlignment(Qt::AlignCenter);
    QFont font = label->font();
    font.setPointSize(16); // Setze die Schriftgröße
    label->setFont(font);

    layout->addWidget(label);
    setLayout(layout);
    //setModal(true);
}
