import sys
import os
from PyQt5 import QtWidgets, QtCore
import paho.mqtt.client as mqtt

sys.path.append(os.getcwd())
from V2.tools import misc  # noqa E402


class ViewTree(QtWidgets.QTreeWidget):
    def __init__(self):
        super().__init__()

        # refresh timer
        self.timer = QtCore.QTimer()
        self.timer.timeout.connect(self.refresh)
        self.timer.start(1000)

        # set qtreewidget
        self.setColumnCount(2)
        self.setHeaderLabels(["Sensor", "Data"])

        # set data dict
        self.dataDict = {}

    def fillTree(self, parent, dataDict={}):
        for _k, _v in dataDict.items():
            _node = QtWidgets.QTreeWidgetItem(parent)
            _node.setExpanded(True)
            _node.setText(0, _k)
            if isinstance(_v, dict):
                self.fillTree(_node, _v)
            else:
                _node.setText(1, str(_v))

    def refresh(self):
        self.clear()
        self.fillTree(self, self.dataDict)

    @QtCore.pyqtSlot(dict)
    def updateData(self, dataDict: dict):
        self.dataDict = dict(misc.mergeDicts(self.dataDict, dataDict))
        self.refresh()


class mqttObject(QtCore.QObject):
    newData = QtCore.pyqtSignal(dict)

    def __init__(self, parent):
        super().__init__(parent)
        self.client = mqtt.Client()
        self.parent = parent
        self.client.on_connect = self.on_connect
        self.client.on_message = self.on_message

    def on_connect(self, client, userdata, flags, rc):
        self.client.subscribe("temp/#")
        self.newData.connect(self.parent.updateData)

    def on_message(self, client, userdata, msg):
        keyChain = msg.topic.split('/')
        value = str(msg.payload.decode())
        _dict = {}
        misc.setInDict(_dict, keyChain, value)
        self.newData.emit(_dict)

    def connect(self, ip="192.168.1.100", port=1883):
        self.client.connect(ip, port=port, keepalive=60)
        self.client.loop_start()


if __name__ == "__main__":

    # init pyqt app
    app = QtWidgets.QApplication(sys.argv)
    treeWidget = ViewTree()
    treeWidget.show()

    # init mqtt
    mqttSocket = mqttObject(treeWidget)
    mqttSocket.connect()

    sys.exit(app.exec_())
