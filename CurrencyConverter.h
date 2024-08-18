#include <string>

#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>

class CurrencyConverter : public QWidget {
    Q_OBJECT

public:
    CurrencyConverter(QWidget *parent = nullptr);

private slots:
    void convertCurrency();

private:
    void initUI();
    double getExchangeRate(const std::string &baseCurrency, const std::string &targetCurrency);
    double parseExchangeRate(const std::string &jsonResponse, const std::string &targetCurrency);
    std::string getHttpResponse(const std::string& url);
    
    QLabel *baseCurrencyLabel;
    QLabel *targetCurrencyLabel;
    QLabel *amountLabel;
    QLabel *resultLabel;
    QComboBox *baseCurrencyInput;
    QComboBox *targetCurrencyInput;
    QLineEdit *amountInput;
};
