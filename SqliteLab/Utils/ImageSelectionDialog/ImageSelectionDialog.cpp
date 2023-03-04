#include "ImageSelectionDialog.h"
#include <QFileDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

ImageSelectionDialog::ImageSelectionDialog(const QString& path, QWidget* parent)
    :QDialog{ parent },
     m_view{ new QLabel{ } } {
        setPath(path);
        auto select_image_button = new QPushButton{ "Select" };
        auto accept_button = new QPushButton{ "Ok" };
        auto reject_button = new QPushButton{ "Cancel" };
        connect(select_image_button, &QPushButton::clicked, this, &ImageSelectionDialog::selectNewImage);
        connect(accept_button, &QPushButton::clicked, this, &QDialog::accept);
        connect(reject_button, &QPushButton::clicked, this, &QDialog::reject);


        auto controls_layout = new QHBoxLayout;
            controls_layout->addWidget(reject_button);
            controls_layout->addWidget(accept_button);

        auto dialog_layout = new QVBoxLayout;
        this->setLayout(dialog_layout);
            dialog_layout->addWidget(m_view, 1);
            dialog_layout->addWidget(select_image_button);
            dialog_layout->addLayout(controls_layout);
    }


const QString &ImageSelectionDialog::path() const {
    return m_path;
}
void ImageSelectionDialog::setPath(const QString& path) {
    m_path = path;
    m_view->setPixmap(QPixmap{ m_path });
}
void ImageSelectionDialog::selectNewImage() {
    auto file = QFileDialog::getOpenFileName(this, {}, {}, "Images (*.png *.jpg)");
    if(file.isEmpty())
        return;

    setPath(file);
}
