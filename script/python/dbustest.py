#!/usr/bin/python3
import dbus
from PyQt5.QtCore import QByteArray, QCoreApplication, Q_CLASSINFO, QJsonValue, pyqtSlot, QObject, QTimer, QJsonDocument
from PyQt5.QtDBus import QDBusConnection, QDBusAbstractAdaptor, QDBusInterface

import signal
signal.signal(signal.SIGINT, signal.SIG_DFL)

json = """
{
    "FirstName": "John",
    "LastName": "Doe",
    "Age": 43,
    "Address": {
        "Street": "Downing Street 10",
        "City": "London",
        "Country": "Great Britain"
    },
    "Phone numbers": [
        "+44 1234567",
        "+44 2345678"
    ]
}
"""

arr = QByteArray()
arr.append(json)
doc = QJsonDocument.fromJson(arr)
print(doc['FirstName'].toString())


class Caller(QObject):

    @pyqtSlot()
    def call(self):
        proxy = QDBusInterface('org.receiver', '/', '',
                               QDBusConnection.sessionBus())
        arr = QByteArray(doc.toJson(1))

        proxy.asyncCall('coughtReply', arr)


bus = QDBusConnection.sessionBus()

# for service in dbus.SessionBus().list_names():
# print(service)

app = QCoreApplication([])

caller = Caller()


QTimer.singleShot(200, caller.call)

app.exec_()
