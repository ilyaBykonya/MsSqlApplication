#pragma once
#include <QPointer>
#include <QDialog>
#include <QLabel>

class ImageSelectionDialog : public QDialog {
    Q_OBJECT
private:
    QString m_path;
    QPointer<QLabel> m_view;
public:
    ImageSelectionDialog(const QString& path = {}, QWidget* parent = nullptr);
    const QString& path() const;
private slots:
    void setPath(const QString& path);
    void selectNewImage();
};




