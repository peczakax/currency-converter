#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QUrl>
#include <stdexcept>
#include <string>

#include "CurrencyConverter.h"


std::string CurrencyConverter::getHttpResponse(const std::string& url) {
    QNetworkAccessManager manager;
    QNetworkRequest request(QUrl(QString::fromStdString(url)));
    QEventLoop loop;

    // Send the request
    QNetworkReply* reply = manager.get(request);

    // Connect the finished signal to the event loop's quit slot
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);

    // Wait for the request to finish
    loop.exec();

    // Check for errors
    if (reply->error() != QNetworkReply::NoError) {
        std::string errorMsg = "Failed to fetch URL: " + reply->errorString().toStdString();
        reply->deleteLater();
        throw std::runtime_error(errorMsg);
    }

    // Read the response
    std::string response = reply->readAll().toStdString();
    reply->deleteLater();

    return response;
}
    

