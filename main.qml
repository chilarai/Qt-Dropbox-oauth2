import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.5

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Qt Dropbox oAuth")

    Button{
        text: qsTr("Autheticate Dropbox")
        anchors.centerIn: parent
        onClicked: {
            Dropboxauth.click()
        }
    }
}
