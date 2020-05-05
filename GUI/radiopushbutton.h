#ifndef RADIOPUSHBUTTON_H
#define RADIOPUSHBUTTON_H

#include <QPushButton>
#include <QLabel>
#include <QSize>
#include <QEvent>
#include <QMouseEvent>

class RadioPushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit RadioPushButton(QWidget* parent = nullptr);
    virtual ~RadioPushButton() override = default;
    void setStr(const QString& str);
    void setPix(const QString& url = QString());

protected:
    virtual void enterEvent(QEvent* event) override;
    virtual void leaveEvent(QEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;

private:
    void init();
    void RoundPixmap(const QPixmap& pix);
    void setPix_(const QString& url);
    void setDefaultPix();
    inline QString elideStr(const QString& str);

private:
    QLabel* pixLabel_{nullptr};
    QLabel* strLabel_{nullptr};
    static constexpr int pixW_{ 64 };
    static constexpr int pixH_{ 64 };

    bool isEnter_{false};
};

#endif // RADIOPUSHBUTTON_H
